constant‑current SEPIC (buck‑boost) LED driver with pure analog dimming (no PWM).

### Topology
- SEPIC with LM3478 so it regulates LED current when Vin crosses VLED (3.2–4.2 V battery, ~3.0–3.3 V LED).
- Constant current via low‑side sense resistor and a small op‑amp that translates sense voltage to the LM3478 `FB` reference (1.26 V).
- Analog dimming by varying the op‑amp reference from 0–1.26 V.

### Concrete example (fill in your LED current if different)
- Assumptions: White LED Vf ≈ 3.0–3.2 V at ILED = 350 mA, Vin = 3.2–4.2 V, fSW ≈ 600 kHz.

Key parts
- Controller: LM3478
- MOSFET: 30 V logic‑level, low Qg/Rds(on) (e.g., SiS412DN, IRLML6344, AO3400A class)
- Diode: Schottky, ≥40 V, ≥1 A (e.g., B240A, SS24)
- Inductors: L1 = L2 = 4.7 µH, shielded, Isat ≥ 2 A, low DCR. Coupled inductor optional to reduce ripple.
- SEPIC cap: 10 µF, 16 V, X7R
- Output cap: 22–47 µF, 10 V, low ESR
- Input cap: 22–47 µF, 10 V, low ESR
- Switch current sense (LM3478 `ISEN`): 0.1 Ω (tune per peak current)
- LED current sense (low‑side): Rsense = 0.1 Ω, 0.5 W
- Op‑amp: Rail‑to‑rail 5 V, GBW ≥ 1 MHz (e.g., TLV9061, MCP6001U for lower BW)

Current regulation and analog dim
- LM3478 `FB` ref = 1.26 V. Sense LED current: Vsense = ILED × Rsense.
- With Rsense = 0.1 Ω at 0.35 A → 35 mV. Amplify to 1.26 V with non‑inverting gain ≈ 36.
  - Example op‑amp resistors: Rin = 3.48 kΩ, Rf = 118 kΩ (gain = 1 + 118/3.48 ≈ 34); trim to hit exactly 1.26 V at full current.
- Provide a reference to the inverting input to enable analog dim:
  - Vref_dim range 0–1.26 V sets 0–100% ILED.
  - Generate Vref_dim with a pot to a stable reference (e.g., TLV431‑based 1.25 V) or a DAC from your MCU.
  - Wire as differential amp so the op‑amp drives `FB` = 1.26 V when (A × Vsense) = Vref_dim. As you reduce Vref_dim, ILED reduces proportionally to zero.

Power‑stage sizing (starting values)
- fSW: 600 kHz (use RT from datasheet; ~18 kΩ for ~600 kHz)
- Duty (worst‑case) around 50–60% at Vin ≈ VLED; SEPIC ripple currents similar in L1/L2.
- Inductors: pick ΔIL ≈ 30–40% of average; 4.7 µH is a good start for 600 kHz here.
- SEPIC coupling cap: 10 µF X7R; keep ESR/ESL low and place tight to L1/L2/diode.
- Slope compensation: per datasheet; start with R/C from `ISEN` node as recommended example and tune if subharmonics appear.
- MOSFET/diode voltage stress: choose ≥20–30 V ratings; SEPIC diode sees roughly Vin+Vout.

COMP network
- Start with Rc ≈ 10 kΩ, Cc ≈ 3.3 nF, Cf ≈ 100 pF on `COMP` (fine‑tune for stable transient response once the current loop is active).

Wiring notes
- LED and Rsense at low side to ground for simple sensing; short, Kelvin sense to op‑amp.
- Op‑amp output drives LM3478 `FB`; do not use the normal output voltage divider.
- Keep the high‑di/dt loop (MOSFET–L1–diode–SEPIC cap) extremely tight; use a solid ground plane.
- Use shielded inductors to reduce EMI; add small RC snubbers if needed on switch node.

Dimming behavior
- Smooth, flicker‑free analog 0–100% by sweeping Vref_dim 0–1.26 V.
- For absolute zero current, let Vref_dim go to 0 V; also tie LM3478 `SHDN` to your control if you want hard‑off.

To finalize exact values, I need:
- LED nominal forward voltage at your target max current
- Desired max current (mA)
- Any size/EMI frequency preference (e.g., ≥1 MHz)
- Whether you can include a small op‑amp, or prefer a current‑sense amplifier IC (e.g., INA180/INA199) instead

I’ll then compute precise Rsense, op‑amp gain, RT, inductor/cap selections, and a tested COMP network.