kepler186f-orbital-engine
A physics engine for orbital mechanics and habitability analysis of Kepler-186f.
Written in C17. No external dependencies.


ABOUT THIS PROJECT

This engine was built around a single constraint: use only what is actually
measurable from Earth. Kepler-186f was confirmed in 2014 via transit photometry
(Quintana et al., Science 344, 277). What we know with confidence is its orbital
period, the mass and luminosity of its host star, and a rough estimate of the
planet's radius from transit depth. Everything else in this engine is derived
from those numbers using established physics.

The goal is not to simulate a planet we know well. It is to demonstrate how much
can be extracted from minimal observational data, and where the hard limits of
that inference are. Every assumption is documented. Every default is tunable.


WHAT THIS ENGINE COMPUTES

Orbital mechanics: the semi-major axis is derived from Kepler's Third Law.
The full Keplerian orbit is solved via Newton-Raphson on Kepler's equation,
producing eccentric anomaly, true anomaly, instantaneous velocity (vis-viva),
and radial vs tangential velocity components at any orbital phase. This gives
a physically complete description of the orbit even for non-zero eccentricity.

Stellar flux: incident power per unit area at the planet's orbital distance,
using the inverse-square law. For eccentric orbits the engine computes flux
at periapsis and apoapsis and the fractional variation over one orbit.

Blackbody spectrum: the host star's emission is modeled as a Planck blackbody
at T_eff = 3788 K. The engine integrates spectral radiance over UV, visible,
and infrared bands using Simpson's rule and computes the peak emission wavelength
from Wien's displacement law. Kepler-186 peaks at ~765 nm, deep red, which has
direct implications for photosynthesis and atmospheric photochemistry on any
planet orbiting it.

Equilibrium temperature: the standard blackbody equilibrium temperature assumes
the planet absorbs stellar flux scaled by (1 - albedo) over its cross-section
and re-radiates uniformly over its full surface. This is a lower bound. A 1-layer
atmosphere model (Sagan & Chyba style) gives a more realistic surface temperature
estimate by accounting for atmospheric infrared opacity (emissivity).

Tidal mechanics: the engine estimates the timescale for Kepler-186f to reach
synchronous (tidally locked) rotation using the MacDonald (1964) formulation with
a Love number derived from the assumed rigidity modulus. Tidal heating power
follows the Peale et al. (1979) Io formulation and is only non-zero for e > 0.
The Roche limit is computed as a reference for orbital stability.

Habitable zone: the Kopparapu et al. (2013) model defines four flux-based
boundaries corrected for stellar T_eff. The conservative habitable zone spans
from the Runaway Greenhouse limit (inner) to the Maximum Greenhouse limit (outer).
Kepler-186f at 0.3926 AU sits near the outer edge of the conservative HZ and
inside the optimistic HZ defined by the Recent Venus and Early Mars limits.


BUILD AND USAGE

  $ make
  $ ./kepler186f-orbital-engine

All tunable parameters have physically motivated defaults. The flags are:

  --albedo     <0.0-1.0>    Bond albedo. Default: 0.306 (Earth proxy).
  --greenhouse <K>          Greenhouse forcing delta in Kelvin. Default: 33.0 K.
  --emissivity <0.0-1.0>    Atmospheric emissivity for 1-layer model. Default: 0.78.
  --ecc        <0.0-0.99>   Orbital eccentricity. Default: 0.0.
  --tidal-q    <value>      Tidal dissipation factor Q. Default: 100.

The eccentricity of Kepler-186f is currently unconstrained by observations.
The albedo has no direct measurement. Both defaults are the most conservative
reasonable assumption given the available data.


KNOWN LIMITATIONS

Eccentricity is unknown. Any non-zero value changes the flux variation,
tidal heating, and orbital state samples significantly. The e=0 default
produces the minimum possible tidal heating (zero) and flat flux.

The tidal lock timescale formula is order-of-magnitude only. It depends
strongly on the initial spin period, which is unobservable, set here to
1 rad/s as a fast-rotator proxy. The system age (~4 Gyr) is itself poorly
constrained, so whether the planet is actually locked is genuinely unknown.

Planet mass is not measured. The radial velocity signal at this orbital
period and stellar mass is below current detection thresholds. The 1.5 M_earth
assumption affects tidal calculations and nothing else.

The greenhouse model is not coupled to atmospheric composition. It applies
a fixed temperature delta, which is useful for sensitivity analysis but not
a substitute for a proper radiative-convective model.

Spectral classification of Kepler-186 is boundary-dependent. The code
assigns K-type at 3788 K using a hard cutoff at 3700 K. Many published
sources classify it as M1V. The boundary is a smooth transition in practice.


REFERENCES

Quintana, E.V. et al. (2014). An Earth-Sized Planet in the Habitable Zone
of a Cool Star. Science 344, 277-280.
https://doi.org/10.1126/science.1249403
https://arxiv.org/abs/1404.5667

Kopparapu, R.K. et al. (2013). Habitable Zones Around Main-Sequence Stars:
Dependence on Planetary Mass. The Astrophysical Journal, 765, 131.
https://doi.org/10.1088/0004-637X/765/2/131
https://arxiv.org/abs/1301.6010

MacDonald, G.J.F. (1964). Tidal Friction.
Reviews of Geophysics, 2(3), 467-541.
https://doi.org/10.1029/RG002i003p00467

Peale, S.J., Cassen, P., Reynolds, R.T. (1979). Melting of Io by Tidal
Dissipation. Science 203, 892-894.
https://doi.org/10.1126/science.203.4383.892

Barnes, R. et al. (2010). Tidal Limits to Planetary Habitability.
The Astrophysical Journal Letters, 709, L95.
https://doi.org/10.1088/2041-8205/709/2/L95
https://arxiv.org/abs/0911.3912

Danby, J.M.A. (1988). Fundamentals of Celestial Mechanics (2nd ed.).
Willmann-Bell. Reference for the Newton-Raphson Kepler equation solver
and Danby initial guess used in kepler.c.

Murray, C.D., Dermott, S.F. (1999). Solar System Dynamics.
Cambridge University Press.
https://doi.org/10.1017/CBO9781139174817

Mamajek, E.E. et al. (2015). IAU 2015 Resolution B3. Nominal conversion
constants for stellar and planetary astronomy.
https://arxiv.org/abs/1510.07674

CODATA 2018 recommended values of physical constants.
https://physics.nist.gov/cuu/Constants/

IAU 2012 definition of the astronomical unit.
https://www.iau.org/static/resolutions/IAU2012_English.pdf

NASA Exoplanet Archive, Kepler-186f entry.
https://exoplanetarchive.ipac.caltech.edu/overview/Kepler-186f

Habitable Exoplanets Catalog, University of Puerto Rico at Arecibo.
https://phl.upr.edu/projects/habitable-exoplanets-catalog

Wikipedia. Kepler-186f.
https://en.wikipedia.org/wiki/Kepler-186f

Wikipedia. Kepler's laws of planetary motion.
https://en.wikipedia.org/wiki/Kepler%27s_laws_of_planetary_motion

Wikipedia. Planetary equilibrium temperature.
https://en.wikipedia.org/wiki/Planetary_equilibrium_temperature

Wikipedia. Tidal locking.
https://en.wikipedia.org/wiki/Tidal_locking

Wikipedia. Planck's law.
https://en.wikipedia.org/wiki/Planck%27s_law

Wikipedia. Wien's displacement law.
https://en.wikipedia.org/wiki/Wien%27s_displacement_law

Wikipedia. Roche limit.
https://en.wikipedia.org/wiki/Roche_limit

Wikipedia. Habitable zone.
https://en.wikipedia.org/wiki/Circumstellar_habitable_zone


LICENSE

MIT License

Copyright (c) 2026 pa

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
