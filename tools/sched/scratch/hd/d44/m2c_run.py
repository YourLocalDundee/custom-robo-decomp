import sys
sys.path.insert(0, 'D:/emulators/gameTesting-hsd2/tools/m2c')
import re
import m2c.translate as _t
_fmt = _t.StructAccess.format


def _target_size(e):
    # the element size of the C type of a dereferenced expression; 0 for an integer (the compiler cannot dereference
    # it: the repair rules would guess s32), None when unknown or an address constant (left as m2c prints it)
    e = _t.late_unwrap(e)
    if isinstance(e, _t.Literal) or (isinstance(e, _t.Cast) and isinstance(_t.late_unwrap(e.expr), _t.Literal)):
        return None
    if isinstance(e, _t.Cast) and e.silent:
        return _target_size(e.expr)
    if isinstance(e, _t.BinaryOp) and e.op in ('+', '-'):
        for side in (e.left, e.right):
            if _t.late_unwrap(side).type.is_pointer_or_array():
                return _target_size(side)
        return 0   # no side typed as a pointer by m2c: the C type is the draft's void * / integer
    ty = e.type
    if ty.is_array() or ty.is_pointer():
        tgt = ty.data().ptr_to
        if tgt is None or tgt.is_void():
            return 0   # void *: the repair rules would guess s32 where m2c knows f32 (fn_8008CDE4)
        return tgt.get_size_bytes() if not tgt.is_struct() else None
    return 0 if ty.is_int() else None


def _scalar(ty):
    if ty.is_int() or ty.is_float():
        return True
    tgt = ty.get_pointer_target() if ty.is_pointer() else None
    return tgt is not None and not tgt.is_function()


def _width_format(self, fmt):
    s = _fmt(self, fmt)
    if not fmt.valid_syntax or self.offset != 0:
        return s
    var = _t.late_unwrap(self.struct_var)
    want = self.type.get_size_bytes()
    if not s.startswith('*'):
        # step 69: m2c's own array spelling of `*(a + i)`, `lbl_801F10C0[(x * 4) & 0xFC]` on a u8[] where the code
        # loads a word (fn_8001B3F0): the access's width at that byte offset
        if want in (2, 4) and isinstance(var, _t.BinaryOp) and var.op == '+' and _target_size(var.left) == 1 \
                and _t.late_unwrap(var.right).type.is_int() and s.startswith(var.left.format(fmt) + '[') \
                and not re.fullmatch(r'sp[0-9A-F]+', _t.format_expr(var.right, fmt)):   # a stack slot m2c reuses as a pointer
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else ('u32 *' if want == 4 else 'u16 *')
            return 'M2C_FIELD(%s, %s, %s)' % (var.left.format(fmt), t, _t.format_expr(var.right, fmt))
        inner = _t.late_unwrap(var.expr) if isinstance(var, _t.AddressOf) else None
        if want in (2, 4) and isinstance(inner, _t.ArrayAccess) and _target_size(inner.ptr) == 1 \
                and _t.late_unwrap(inner.index).type.is_int() \
                and not re.fullmatch(r'sp[0-9A-F]+', _t.format_expr(inner.index, fmt)):
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else ('u32 *' if want == 4 else 'u16 *')
            return 'M2C_FIELD(%s, %s, %s)' % (inner.ptr.format(fmt), t, _t.format_expr(inner.index, fmt))
        return s
    if not want or s not in ('*' + var.format(fmt), '*(' + var.format(fmt) + ')'):
        return s
    if isinstance(var, _t.Cast) and not var.silent and var.type.is_pointer() and want in (1, 2, 4, 8):
        # step 80: `*((cr_Pad *) ((u8 *) p + off)) = 0` (fn_80020204: a byte store at a variable offset into the typed
        # array; m2c casts the address to the element type and the compiler cannot assign an int to a struct): the
        # access with its own width
        tgt = var.type.get_pointer_target()
        if tgt is not None and tgt.is_struct():
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else {1: 'u8 *', 2: 'u16 *', 4: 'u32 *', 8: 'u64 *'}[want]
            return 'M2C_FIELD(%s, %s, 0)' % (var.expr.format(fmt), t)
    if not _scalar(self.type):
        # step 69: a word / halfword m2c could not tell from a pointer (its int-or-pointer type) read from a byte array
        # (`(u32) *(lbl_8024E120 + i * 0x33520) != 0`, dtk's untyped data: a lbz where the original has lwz:
        # fn_8000768C, fn_800BC9A4, fn_8010D2D0): the access's own width; step 80: also through a `void *` / integer
        # base (`*(arg0 + i * 4 + 0xAC)` of a `void *` parameter that the repair loop makes `u8 *`: fn_8008688C's
        # lwzx / stwx came out as lbz / stb)
        if want in (2, 4) and _target_size(var) in (0, 1):
            return _split_field(var, 'u32 *' if want == 4 else 'u16 *', fmt)
        return s
    if isinstance(var, _t.BinaryOp) and var.op == '+':
        # step 69: `*(chara->motion_set + i * 2)` on a struct pointer (m2c's offset is in bytes, C would scale it by the
        # struct): the access with its own width at that byte offset (cr_C_ACT_MOT: lhzx, the repair rule read a u32)
        for p, n in ((var.left, var.right), (var.right, var.left)):
            pt = _t.late_unwrap(p).type
            tgt = pt.get_pointer_target() if pt.is_pointer() else None
            if tgt is not None and tgt.is_struct() and _t.late_unwrap(n).type.is_int():
                return 'M2C_FIELD(%s, %s, %s)' % (p.format(fmt), _t.Type.ptr(self.type).format(fmt), _t.format_expr(n, fmt))
    have = _target_size(var)
    if have is None or have == want:
        return s
    return _split_field(var, _t.Type.ptr(self.type).format(fmt), fmt)


def _split_field(var, t, fmt):
    # step 80, cycle 2 tried `*(p + k)` -> M2C_FIELD(p, T *, k) (the offset as the macro's argument: an indexed load with
    # the sum in a register, fn_8008688C's lwzx); it lost 16 matches whose originals fold the constant of k into the
    # displacement (fn_8007A1D0, fn_800AB468 families: 100% -> 82-84%), so the whole sum stays at offset 0
    return 'M2C_FIELD(%s, %s, 0)' % (var.format(fmt), t)


_afmt = _t.ArrayAccess.format


def _array_width_format(self, fmt):
    # `lbl_801F10C0[i]` on a u8[] where the code loads a word: the index is a byte offset there
    want = self.type.get_size_bytes()
    if fmt.valid_syntax and want and want > 1 and _target_size(self.ptr) == 1:
        if _scalar(self.type):
            return 'M2C_FIELD(%s, %s, %s)' % (self.ptr.format(fmt), _t.Type.ptr(self.type).format(fmt), _t.format_expr(self.index, fmt))
        if want in (2, 4):   # step 69: m2c's int-or-pointer word (fn_8001B3F0), as in _width_format
            return 'M2C_FIELD(%s, %s, %s)' % (self.ptr.format(fmt), 'u32 *' if want == 4 else 'u16 *', _t.format_expr(self.index, fmt))
    return _afmt(self, fmt)


_t.StructAccess.format = _width_format
_t.ArrayAccess.format = _array_width_format
from m2c.main import main
main()
