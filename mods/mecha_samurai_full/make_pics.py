"""The mecha samurai's pictures: the Parts Index pictures of his body, gun, bomb, pod and legs, and the ending picture.

  tools/pyenv/Scripts/python -I mods/mecha_samurai_full/make_pics.py

Each picture replaces the stock one of the part it stands in for (body 0 Ray 01 = SF011, Basic Gun, Standard Bomb /
Pod / Legs), written in the stock file's own format and size by tools/tpl.py: Parts Index pictures are 240x240 CMPR
on black, the ending picture 288x327 CI8 with an RGB5A3 palette (a cut-out with a glow and the name below, as Ray 01's).
Sources: D:\\ai\\Pictures\\mecha_samurai_parts (FLUX 3 Klein pictures; pose_s7 = the battle pose).
"""
import sys
from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw, ImageFilter, ImageFont

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
sys.path.insert(0, str(G / 'tools'))
import tpl  # noqa: E402

AI = Path(r'D:\ai\Pictures\mecha_samurai_parts')
SP = G / 'Custom Robo extracted' / 'files' / 'sprite'
OUT = MOD / 'files' / 'sprite'
WORK = G / 'build' / 'samurai_full' / 'pics'


def cutout(im, bg):
    """alpha from the distance to a plain background (white or black), the object cropped with a margin"""
    a = np.asarray(im.convert('RGB')).astype(np.float32)
    ref = np.array([255.0, 255, 255]) if bg == 'white' else np.array([16.0, 16, 16])
    dist = np.abs(a - ref).max(-1)
    alpha = np.clip((dist - (18 if bg == 'white' else 52)) / 30, 0, 1)   # the black pose picture has a grey floor
    rgba = np.dstack([a, alpha * 255]).astype(np.uint8)
    img = Image.fromarray(rgba, 'RGBA')
    box = Image.fromarray((alpha > 0.5).astype(np.uint8) * 255).getbbox()
    return img.crop(box)


def fit(img, w, h, margin, bg=(0, 0, 0, 255)):
    out = Image.new('RGBA', (w, h), bg)
    s = min((w - 2 * margin) / img.width, (h - 2 * margin) / img.height)
    t = img.resize((max(1, int(img.width * s)), max(1, int(img.height * s))), Image.LANCZOS)
    out.alpha_composite(t, ((w - t.width) // 2, (h - t.height) // 2))
    return out


def write(stock_rel, png):
    src = SP / stock_rel
    dst = OUT / stock_rel
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_bytes(tpl.encode(src.read_bytes(), png))
    tpl.decode(dst.read_bytes()).save(WORK / (Path(stock_rel).stem + '.check.png'))
    print('wrote', dst.relative_to(MOD))


def main():
    WORK.mkdir(parents=True, exist_ok=True)
    pose = cutout(Image.open(AI / 'pose_s7_00001_.png'), 'black')
    # Parts Index: on black, filling the square
    p = WORK / 'index_body.png'
    fit(pose, 240, 240, 6).save(p)
    write('parts_illustrated_book/body_tpl/CR_S1_SF011.tpl', p)
    for part, img, rel in (('gun', 'klein_gun_s29_00001_.png', 'gun_tpl/WPG_S1_BASIC.tpl'),
                           ('bomb', 'klein_bomb_s11_00001_.png', 'bomb_tpl/WPB_S1_STANDARD.tpl'),
                           ('pod', 'klein_pod_s29_00001_.png', 'pod_tpl/WPP_S1_STANDARD.tpl'),
                           ('legs', 'klein_legs2_s11_00001_.png', 'leg_tpl/LEG_S1_STND.tpl')):
        p = WORK / f'index_{part}.png'
        fit(cutout(Image.open(AI / img), 'white'), 240, 240, 14).save(p)
        write('parts_illustrated_book/' + rel, p)
    # ending: transparent, the robo with a pale glow, the name in yellow below (Ray 01's layout)
    W, H = 288, 327
    fig = fit(pose, W, H - 40, 4, bg=(0, 0, 0, 0))
    canvas = Image.new('RGBA', (W, H), (0, 0, 0, 0))
    glow = Image.new('RGBA', (W, H), (0, 0, 0, 0))
    glow.paste(fig, (0, 0), fig)
    a = np.asarray(glow.split()[3].filter(ImageFilter.GaussianBlur(5))).astype(np.float32) * 0.8
    g = Image.fromarray(np.dstack([np.full((H, W), 200), np.full((H, W), 235), np.full((H, W), 255), a]).astype(np.uint8), 'RGBA')
    canvas.alpha_composite(g)
    canvas.alpha_composite(fig)
    d = ImageDraw.Draw(canvas)
    try:
        font = ImageFont.truetype(r'C:\Windows\Fonts\arialbd.ttf', 30)
    except OSError:
        font = ImageFont.load_default()
    text = 'SAMURAI'
    tw = d.textlength(text, font=font)
    d.text(((W - tw) / 2, H - 38), text, font=font, fill=(250, 240, 60, 255), stroke_width=2, stroke_fill=(120, 90, 0, 255))
    p = WORK / 'ending.png'
    canvas.save(p)
    write('ending/us/robo_tpl/CR_SF011.tpl', p)


if __name__ == '__main__':
    main()
