# Pilot in the desert

Exploratory character/art packet for the pilot who meets the Little Prince in the desert.

## Character target

The pilot should read as:

- dark-skinned;
- heavyset / fat rather than merely broad-shouldered;
- large black mustache;
- khaki flight suit and practical, worn aviation clothing;
- physically and emotionally exhausted;
- underlying depression / resignation visible in the whole face and posture;
- suddenly **wide-eyed with astonishment** when he sees the Prince.

The key acting idea is not "replace sadness with surprise." Keep the tired, world-weary face and slumped body, then superimpose astonishment: eyes open very wide, brows rise, jaw parts slightly. The old expression remains visible underneath the new one.

Avoid turning the reaction into generic cheerful wonder. He has been alone, exhausted and stranded; seeing the Prince is an interruption of that state.

## Period

Do not lock the visual design to World War I. *The Little Prince* was published in 1943, and the desert-crash imagery is strongly associated with Antoine de Saint-Exupéry's 1935 Sahara crash. A worn interwar / 1930s aviator reference is therefore safer than a specifically WWI uniform.

This is an art reference, not a requirement to make the game pilot a literal portrait of Saint-Exupéry.

## Rendering vocabulary

Desired direction:

- painterly;
- caricatured;
- hand-inked;
- loose linework;
- variable line weight;
- scratchy / imperfect contours;
- organic, irregular shapes;
- weathered surfaces;
- visible texture and grain;
- muted, earthy palette;
- khaki, ochre, rust, dusty blue, faded red;
- watercolor-like backgrounds;
- gouache-like opaque painted color;
- broad painted shadow shapes rather than smooth digital gradients;
- cel-painted / limited shadow levels on the character;
- illustrative and storybook-like;
- old-fashioned / analog-looking animation;
- chunky character design;
- heavy facial features;
- asymmetrical expression;
- lived-in clothes and machinery;
- romantic realism;
- melancholic adventure illustration;
- dusty cinematic atmosphere.

The pilot should have sagging cheeks, heavy eyelids, substantial nose and brow, deep fatigue lines, thick mustache, rounded shoulders, rumpled clothing, and weighty anatomy.

Avoid:

- glossy modern-anime rendering;
- perfectly smooth vector-like outlines;
- plastic skin;
- overly cute proportions;
- a clean heroic flight suit;
- a permanently bright, cheerful expression.

## Animation direction

For a freely explorable 3D game, treat these drawings as reference art for a 3D model rather than attempting to interpolate complete 2D views around the character.

Suggested pipeline:

1. sculpt/model the pilot from several reference views;
2. build a 3D skeletal rig for body motion;
3. skin the mesh to the skeleton;
4. use a dense face rig informed by facial anatomy;
5. expose a smaller set of high-level expression controls to animation/game logic;
6. render with hand-painted textures, limited/toon shading, irregular outlines and subtle texture/line wobble so the 3D character retains the rough painted quality.

The face rig should allow fatigue and astonishment to coexist. See `facial-rig.md`.

## Reference iterations

`contact-sheet.webp` preserves the five visual iterations from the design conversation in one compact in-repository reference:

1. tired mustached pilot, comparatively light skin and thinner body;
2. darker skin and heavier body;
3. darker again, broader cinematic treatment;
4. emphasis on underlying depression plus wide-eyed astonishment at seeing the Prince;
5. rougher painterly / caricatured hand-drawn direction.

The contact sheet is intentionally a compact reference rather than production-resolution artwork. The full generated originals remain exploratory source material rather than final game assets.
