#!/usr/bin/env python3
"""Schema-check pilot Idris interval observations and C midpoint projection.

This validator does NOT substitute for an actual Idris2 typecheck.
"""
import argparse
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
IDRIS = ROOT / "pilot-expressions" / "intervals.idr"
AUDIT = ROOT / "actuator-audit" / "actuators.tsv"
HEADER = ROOT / "android" / "native" / "face_photo_guess.h"
ROW = re.compile(r'^\s*\("([A-Za-z0-9_]+)",\s*(\d+),\s*(\d+),\s*(\d+)\),?\s*$', re.M)


def observations():
    rows = [(name, int(index), int(lo), int(hi))
            for name, index, lo, hi in ROW.findall(IDRIS.read_text(encoding="utf-8"))]
    if len(rows) != 29:
        raise ValueError(f"expected 29 interval observations, got {len(rows)}")
    audit = [line.split("\t") for line in AUDIT.read_text().splitlines()[1:]]
    by_index = {int(row[0]): row[1] for row in audit}
    if len(by_index) != 102:
        raise ValueError("expected canonical 102-actuator ordering")
    unique = set()
    for name, index, lo, hi in rows:
        if index in unique or by_index.get(index) != name:
            raise ValueError(f"duplicate or incorrect native actuator index: {name}/{index}")
        unique.add(index)
        if not (0 <= lo <= hi <= 100):
            raise ValueError(f"invalid activation interval for {name}: {lo}, {hi}")
    by_name = {name: (lo, hi) for name, _, lo, hi in rows}
    for side in ("L", "R"):
        if by_name[f"frontalis_medial_{side}"][0] <= by_name[f"frontalis_lateral_{side}"][1]:
            raise ValueError("medial frontalis must exceed lateral frontalis")
        if by_name[f"corrugator_supercilii_{side}"][0] < 20:
            raise ValueError("brow knit lost")
        if by_name[f"depressor_anguli_oris_{side}"][1] > 35:
            raise ValueError("mouth should remain quiet")
    return rows


def generate(rows):
    result = [
        "#ifndef PILOT_FACE_PHOTO_GUESS_H",
        "#define PILOT_FACE_PHOTO_GUESS_H",
        "/* Generated projection of pilot-expressions/intervals.idr. */",
        "/* Run: python3 pilot-expressions/check_intervals.py --write */",
        "/* Midpoints are native rig activations, NOT measured muscle positions. */",
        "static const double pilot_photo_guess_midpoint[FACE_ACTUATORS] = {",
    ]
    for name, index, lo, hi in rows:
        result.append(f"    [{index}] = {(lo + hi) / 200:.3f}, /* {name} [{lo}, {hi}] percent */")
    return "\n".join(result + ["};", "#endif", ""])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()
    rows = observations()
    expected = generate(rows)
    if args.write:
        HEADER.write_text(expected, encoding="utf-8")
    elif not HEADER.is_file() or HEADER.read_text(encoding="utf-8") != expected:
        raise ValueError("generated native header stale; run check_intervals.py --write")
    print(f"PASS pilot expression: {len(rows)} intervals, 102-actual-actuator mapping")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, KeyError) as exc:
        sys.exit(f"FAIL photo intervals: {exc}")
