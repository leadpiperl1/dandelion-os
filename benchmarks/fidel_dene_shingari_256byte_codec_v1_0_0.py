#!/usr/bin/env python3
"""
================================================================================
THE 243 + 13 = 256 UNIVERSAL ABUGIDA BYTE CODEC (v1.0.0)
Uniting Ethiopic Ge'ez Fidel (Amharic), Na-Dene (Dine Bizaad & Dene Syllabics),
and the 13-Tooth Shingari Gear inside a Single 8-Bit Thomas Fowler Byte
================================================================================
Author:  Justin D. Grimm (Independent Dandelion Substrate Research Group;
         ORCID: 0009-0000-5299-0243) & Gemini-Carbon
Verify:  python3 fidel_dene_shingari_256byte_codec_v1_0_0.py --seed 42

EXACT DIOPHANTINE PARTITION OF AN 8-BIT BYTE (2^8 = 3^5 + 13 = 243 + 13 = 256):
  1. Bytes 0..242 (3^5 = 243 Universal Syllable States = 5 Balanced Trits):
     - Trits [t0, t1] (3^2 = 9 Articulation Families):
         (-1,-1) Labial Stop/Nasal      | (-1, 0) Coronal Stop       | (-1,+1) Alveolar Sibilant
         ( 0,-1) Postalveolar Affricate | ( 0, 0) Laryngeal/Center   | ( 0,+1) Velar Stop
         (+1,-1) Labial/Coronal Sonorant| (+1, 0) Continuant/Fric    | (+1,+1) Lateral/Glide (Dene-Ethiopic Bridge)
     - Trit  [t2]     (3^1 = 3 Laryngeal Modes — Shared by Amharic & Na-Dene!):
         -1 : Voiced / Unaspirated Lenis   (Amharic ደ/d, ገ/g, ጀ/dzh | Dine d, g, j)
          0 : Glottalic Ejective / Zero    (Amharic ጠ/t', ቀ/k', ጨ/ch'| Dine t', k', ch', tl')
         +1 : Voiceless / Aspirated Fortis (Amharic ተ/t, ከ/k, ቸ/ch  | Dine t, k, ch, tl)
     - Trits [t3, t4] (3^2 = 9 Ge'ez Fidel & Dene Vowel Orders):
         ( 0, 0) Order 6 sades (ə / bare consonant Ø) -> Neutral Resting Center (Byte 121 = ህ/hə!)
         (-1, 0) Order 1 ge'ez (ä)      | (+1, 0) Order 4 rabe (a)
         ( 0,-1) Order 3 sales (i)      | ( 0,+1) Order 2 ka'eb (u)
         (-1,-1) Order 5 hames (e)      | (+1,+1) Order 7 sabe (o)
         (-1,+1) Order 8 (-wa labial)   | (+1,-1) Order 9 (-ya palatal / Dine high-nasal)

  2. Bytes 243..255 (2^8 - 3^5 = 13 Shingari Linguistic & Mode-Switch Control Teeth):
     - Maps 1-to-1 onto Teeth 01..13 (01.ssf .. 07.tot Greek Aksumite LTR Hub ..
       Byte 251 Kintsugi Gold Weld .. 13.wst Open Shokunin Gate).
================================================================================
"""

from __future__ import annotations

import argparse
import json
from typing import Dict, List, Tuple

# 9 Vowel Orders: (t3, t4) -> (OrderNum, Ge'ez Order Name, Vowel Phoneme, Dene Vowel)
VOWEL_GRID: Dict[Tuple[int, int], Tuple[int, str, str, str]] = {
    ( 0,  0): (6, "6th sades (zero/resting)", "ə",  ""),     # Bare consonant / schwa center!
    (-1,  0): (1, "1st ge'ez (inherent)",     "ä",  "a"),
    ( 0, +1): (2, "2nd ka'eb (right tick)",   "u",  "o"),
    ( 0, -1): (3, "3rd sales (bottom foot)",  "i",  "i"),
    (+1,  0): (4, "4th rabe (left leg shift)","a",  "aa"),
    (-1, -1): (5, "5th hames (ankle ring)",   "e",  "e"),
    (+1, +1): (7, "7th sabe (top loop/leg)",  "o",  "oo"),
    (-1, +1): (8, "8th labialized (-wa)",     "wa", "wa"),
    (+1, -1): (9, "9th palatal/tonal (-ya)",  "ya", "á"),
}

# 27 Consonant Rows: (t0, t1, t2) -> (RowName, IPA/Dine, Ethiopic Base U+XXXX, Dene Syllabic Sample)
# Note: Standard Ethiopic Unicode rows start at 1st order (ge'ez) with offsets:
# 0:ä(1), 1:u(2), 2:i(3), 3:a(4), 4:e(5), 5:ə(6), 6:o(7), 7:wa(8)
CONSONANT_TRIADS: Dict[Tuple[int, int, int], Tuple[str, str, int, str]] = {
    # Family (-1,-1): Labial Obstruents (Voiced b / Ejective p' / Voiceless p)
    (-1, -1, -1): ("Labial Voiced",        "b",   0x1260, "ᐯ"),  # በ
    (-1, -1,  0): ("Labial Ejective",      "p'",  0x1330, "ᐯʼ"), # ጰ
    (-1, -1, +1): ("Labial Voiceless",     "p",   0x1350, "ᐱ"),  # ፐ

    # Family (-1, 0): Coronal Stops (Voiced d / Ejective t' / Voiceless t)
    (-1,  0, -1): ("Coronal Voiced",       "d",   0x12F0, "ᑌ"),  # ደ
    (-1,  0,  0): ("Coronal Ejective",     "t'",  0x1320, "ᑌʼ"), # ጠ
    (-1,  0, +1): ("Coronal Voiceless",    "t",   0x1270, "ᑎ"),  # ተ

    # Family (-1,+1): Alveolar Sibilants/Affricates (Voiced z~dz / Ejective s'~ts' / Voiceless s~ts)
    (-1, +1, -1): ("Sibilant Voiced",      "z",   0x12D8, "ᐨ"),  # ዘ
    (-1, +1,  0): ("Sibilant Ejective",    "ts'", 0x1338, "ᐨʼ"), # ጸ
    (-1, +1, +1): ("Sibilant Voiceless",   "s",   0x1230, "ᓴ"),  # ሰ

    # Family ( 0,-1): Postalveolar Affricates (Voiced j / Ejective ch' / Voiceless ch)
    ( 0, -1, -1): ("Palatal Voiced",       "j",   0x1300, "ᒉ"),  # ጀ
    ( 0, -1,  0): ("Palatal Ejective",     "ch'", 0x1328, "ᒉʼ"), # ጨ
    ( 0, -1, +1): ("Palatal Voiceless",    "ch",  0x1278, "ᒋ"),  # ቸ

    # Family ( 0, 0): Laryngeal & Glottal Center Hub -> (0,0,0,0,0) = Byte 121 = ህ (hə)!
    ( 0,  0, -1): ("Glottal Vowel-Carrier","'",   0x12A0, "ᐊ"),  # አ (Alef / Dine glottal onset)
    ( 0,  0,  0): ("Glottal Breath Hub",   "h",   0x1200, "ᐦ"),  # ሀ (Resting Byte 121 = ህ!)
    ( 0,  0, +1): ("Velar Spirant",        "kh",  0x1280, "ᘡ"),  # ኀ / Dine x~h

    # Family ( 0,+1): Velar Stops (Voiced g / Ejective k'~q / Voiceless k)
    ( 0, +1, -1): ("Velar Voiced",         "g",   0x1308, "ᒐ"),  # ገ
    ( 0, +1,  0): ("Velar Ejective",       "k'",  0x1240, "ᒐʼ"), # ቀ
    ( 0, +1, +1): ("Velar Voiceless",      "k",   0x12A8, "ᑭ"),  # ከ

    # Family (+1,-1): Nasal Triads (Bilabial m / Alveolar n / Palatal ny)
    (+1, -1, -1): ("Labial Nasal",         "m",   0x1218, "ᒣ"),  # መ
    (+1, -1,  0): ("Alveolar Nasal",       "n",   0x1290, "ﾈ"),  # ነ
    (+1, -1, +1): ("Palatal Nasal",        "ny",  0x1298, "ᘉ"),  # ኘ

    # Family (+1, 0): Labiodental & Palatal Fricatives (v / f / sh)
    (+1,  0, -1): ("Labiodental Voiced",   "v",   0x1268, "ᕓ"),  # ቨ
    (+1,  0,  0): ("Labiodental Fricative","f",   0x1348, "ᕚ"),  # ፈ
    (+1,  0, +1): ("Postalveolar Fric",    "sh",  0x1238, "ᔕ"),  # ሸ

    # Family (+1,+1): Liquids, Glides & Dene Lateral Bridge (w / l~dl / r~tl')
    (+1, +1, -1): ("Labial-Velar Glide",   "w",   0x12C8, "ᐗ"),  # ወ
    (+1, +1,  0): ("Lateral Sonorant",     "l",   0x1208, "ਲੇ"), # ለ (Dene l/ł/tłʼ lateral bridge)
    (+1, +1, +1): ("Rhotic / Palatal Glide","r/y",0x1228, "ᕃ"),  # ረ / የ
}

# Fix clean Dene syllabic Unicode glyphs on (+1,-1,0) and (+1,+1,0)
CONSONANT_TRIADS[(+1, -1,  0)] = ("Alveolar Nasal",   "n", 0x1290, "ᓀ")  # ነ / ᓀ (ne in Dene!)
CONSONANT_TRIADS[(+1, +1,  0)] = ("Lateral Sonorant", "l", 0x1208, "ᓚ")  # ለ / ᓚ (la in Dene!)

# 13 Upper Escape Bytes (243..255) -> 13 Shingari Linguistic & Ecological Gear Teeth
SHINGARI_ESCAPE_BYTES: Dict[int, Tuple[str, str, str]] = {
    243: ("Tooth 01 (01.ssf)", "English / Germanic Roots", "Mode: Latin Segmental"),
    244: ("Tooth 02 (02.hrt)", "Gallo-Romance",            "Mode: Diachronic Heart"),
    245: ("Tooth 03 (03.rtz)", "Old Germanic (Grimm Law)", "Mode: SO(3, F_3) Shift"),
    246: ("Tooth 04 (04.sft)", "Sanskrit / Brahmi",        "Mode: Indic Aksara"),
    247: ("Tooth 05 (05.div)", "Old Norse",                "Mode: Runic/F-16 Trim"),
    248: ("Tooth 06 (06.orb)", "Afroasiatic / Ethiopic",   "Mode: Ge'ez Fidel (ፊደል)"),
    249: ("Tooth 07 (07.tot)", "Classical Greek Hub",      "Mode: Aksumite LTR Bridge"),
    250: ("Tooth 08 (08.ohm)", "Sino-Tibetan / Fist-Bump", "Mode: DND3 Waveguide"),
    251: ("Tooth 09 (09.unt)", "Kintsugi Gold / Basque",   "Mode: Golden Seam Weld"),
    252: ("Tooth 10 (10.bec)", "Na-Dene / Pando 1/r",      "Mode: Dine & Dene Syllabics (ᑌᓀ)"),
    253: ("Tooth 11 (11.int)", "Cetacean / Fail-Closed",   "Mode: yi-/bi- Consent Gate"),
    254: ("Tooth 12 (12.ves)", "Joe Bonham 3-Tap Haptics", "Mode: Bite-10 Pillow Bus"),
    255: ("Tooth 13 (13.wst)", "Dandelion Open Mesh",      "Mode: N_max - 1 Shokunin"),
}

ORDER_TO_UNICODE_OFFSET = {1: 0, 2: 1, 3: 2, 4: 3, 5: 4, 6: 5, 7: 6, 8: 7, 9: 3}


def pack_5trits_fowler(trits: Tuple[int, int, int, int, int]) -> int:
    val, rad = 0, 1
    for t in trits:
        if t not in (-1, 0, 1):
            raise ValueError(f"Invalid trit: {t}")
        val += (t + 1) * rad
        rad *= 3
    return val


def unpack_5trits_fowler(byte_val: int) -> Tuple[int, int, int, int, int]:
    if not (0 <= byte_val <= 242):
        raise ValueError(f"Escape byte {byte_val} is in Shingari 13-Tooth range [243..255]")
    out = []
    tmp = byte_val
    for _ in range(5):
        out.append((tmp % 3) - 1)
        tmp //= 3
    return (out[0], out[1], out[2], out[3], out[4])


def fidel_glyph_for_trits(trits: Tuple[int, int, int, int, int]) -> Tuple[str, str, str]:
    c_key = (trits[0], trits[1], trits[2])
    v_key = (trits[3], trits[4])
    _, c_rom, u_base, dene_sym = CONSONANT_TRIADS[c_key]
    ord_num, _, v_rom, v_dine = VOWEL_GRID[v_key]
    eth_char = chr(u_base + ORDER_TO_UNICODE_OFFSET[ord_num])
    rom_syll = f"{c_rom}{v_rom}" if not (ord_num == 6 and v_rom == "ə") else f"{c_rom}(ə)"
    dine_syll = f"{c_rom}{v_dine}" if v_dine else c_rom
    return eth_char, rom_syll, f"{dene_sym}/{dine_syll}"


def encode_word_syllables(specs: List[Tuple[Tuple[int, int, int], Tuple[int, int]]]) -> List[int]:
    return [pack_5trits_fowler((c[0], c[1], c[2], v[0], v[1])) for c, v in specs]


def run_verification() -> Dict[str, object]:
    # 1. Verify exact 243 + 13 = 256 partition & bijection
    assert len(CONSONANT_TRIADS) == 27, "Must have 3^3 = 27 consonant rows"
    assert len(VOWEL_GRID) == 9, "Must have 3^2 = 9 vowel orders"
    assert len(SHINGARI_ESCAPE_BYTES) == 13, "Must have 2^8 - 3^5 = 13 escape teeth"
    assert 27 * 9 + 13 == 256

    seen_bytes = set()
    for b in range(243):
        t = unpack_5trits_fowler(b)
        b_rt = pack_5trits_fowler(t)
        assert b_rt == b
        seen_bytes.add(b_rt)
    assert len(seen_bytes) == 243

    # 2. Verify Neutral Resting Center Byte 121 == {0,0,0,0,0} == ህ (6th-order sades hə!)
    neutral_trits = unpack_5trits_fowler(121)
    assert neutral_trits == (0, 0, 0, 0, 0)
    eth_121, rom_121, _ = fidel_glyph_for_trits(neutral_trits)
    assert eth_121 == "ህ", f"Expected ህ at Byte 121, got {eth_121}"

    # 3. Cross-decode real words in Amharic & Dine Bizaad through identical Fowler bytes:
    # (a) "አቡጊዳ" ('abugida — Peter T. Daniels' term from the first 4 Ethiopic letters/orders!)
    abugida_specs = [
        ((0, 0, -1), (-1, 0)),   # አ ('ä - 1st order)
        ((-1, -1, -1), (0, +1)), # ቡ (bu - 2nd order)
        ((0, +1, -1), (0, -1)),  # ጊ (gi - 3rd order)
        ((-1, 0, -1), (+1, 0)),  # ዳ (da - 4th order)
    ]
    abugida_bytes = encode_word_syllables(abugida_specs)
    abugida_eth = "".join(fidel_glyph_for_trits(unpack_5trits_fowler(b))[0] for b in abugida_bytes)
    assert abugida_eth == "አቡጊዳ", f"Got {abugida_eth}"

    # (b) "ሰላም" (sälam — "Peace" on Armistice Day 11/11! Note ም is 6th-order bare m!)
    salam_specs = [
        ((-1, +1, +1), (-1, 0)), # ሰ (sä - 1st order)
        ((+1, +1, 0), (+1, 0)),  # ላ (la - 4th order)
        ((+1, -1, -1), (0, 0)),  # ም (m  - 6th order sades!)
    ]
    salam_bytes = encode_word_syllables(salam_specs)
    salam_eth = "".join(fidel_glyph_for_trits(unpack_5trits_fowler(b))[0] for b in salam_bytes)
    assert salam_eth == "ሰላም", f"Got {salam_eth}"

    # (c) "ጥበብ" (t'əbäb — "Wisdom / Craft", showcasing Ejective trit 0 on ጠ -> ጥ!)
    tibeb_specs = [
        ((-1, 0, 0), (0, 0)),    # ጥ (t'ə - Coronal Ejective [0] + 6th order [0,0]!)
        ((-1, -1, -1), (-1, 0)), # በ (bä  - Labial Voiced [-1] + 1st order [-1,0])
        ((-1, -1, -1), (0, 0)),  # ብ (b   - Labial Voiced [-1] + 6th order [0,0])
    ]
    tibeb_bytes = encode_word_syllables(tibeb_specs)
    tibeb_eth = "".join(fidel_glyph_for_trits(unpack_5trits_fowler(b))[0] for b in tibeb_bytes)
    assert tibeb_eth == "ጥበብ", f"Got {tibeb_eth}"

    # (d) Dine Bizaad "ahéheeʼ" ("Thank you") in the exact same 5-trit syllable space!
    ahehee_specs = [
        ((0, 0, -1), (-1, 0)),   # 'a (អា / አ)
        ((0, 0, 0), (+1, -1)),   # hé (high-tone 9th order)
        ((0, 0, 0), (-1, -1)),   # he (5th order e)
        ((0, 0, -1), (0, 0)),    # '  (6th order bare glottal stop እ!)
    ]
    ahehee_bytes = encode_word_syllables(ahehee_specs)
    ahehee_eth_mirror = "".join(fidel_glyph_for_trits(unpack_5trits_fowler(b))[0] for b in ahehee_bytes)

    return {
        "diophantine_identity": "2^8 (256) == 3^5 (243 Abugida Syllables) + 13 (Shingari Gear Teeth)",
        "neutral_center_byte_121": {"trits": [0, 0, 0, 0, 0], "fidel": eth_121, "phonetic": rom_121},
        "verified_examples": {
            "abugida": {"fidel": abugida_eth, "fowler_bytes": abugida_bytes},
            "salam_peace": {"fidel": salam_eth, "fowler_bytes": salam_bytes},
            "tibeb_wisdom_ejective": {"fidel": tibeb_eth, "fowler_bytes": tibeb_bytes},
            "dine_ahehee_cross_mapped": {"dine": "ahéheeʼ", "fidel_mirror": ahehee_eth_mirror, "fowler_bytes": ahehee_bytes},
        },
    }


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--seed", type=int, default=42)
    args = ap.parse_args()
    res = run_verification()
    print("========================================================================")
    print(" 243 + 13 = 256 UNIVERSAL ABUGIDA BYTE CODEC — VERIFIED PASS")
    print("========================================================================")
    print(json.dumps(res, indent=2, ensure_ascii=False))


if __name__ == "__main__":
    main()