#!/usr/bin/env python3
"""Run every deterministic pilot geometry/topology check."""
from __future__ import annotations
from pathlib import Path
import subprocess
import sys

HERE=Path(__file__).resolve().parent
SCRIPTS=("generate_control_cage.py","generate_surface_topology.py","generate_feature_topology.py")
for script in SCRIPTS:
    subprocess.run([sys.executable,str(HERE/script),"--check"],check=True)
print("pilot geometry/topology checks: PASS")
