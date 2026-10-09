module Main

import Data.Fin
import Data.Nat
import Data.Vect

%default total

-- Closed [lower, upper] in integer hundredths of the model's q in [0,1].
-- The proofs guarantee lower <= upper <= 100 at construction time.
-- Not a measured biological contraction percentage or a probability interval.
data ActivationInterval : Type where
  Closed : (lowerPercent : Nat) -> (upperPercent : Nat) ->
           LTE lowerPercent upperPercent -> LTE upperPercent 100 ->
           ActivationInterval

checkedInterval : (lo : Nat) -> (hi : Nat) -> Maybe ActivationInterval
checkedInterval lo hi =
  case (isLTE lo hi, isLTE hi 100) of
    (Yes ordered, Yes bounded) => Just (Closed lo hi ordered bounded)
    _ => Nothing

midpoint : ActivationInterval -> Double
midpoint (Closed lo hi _ _) = (cast lo + cast hi) / 200.0

record MuscleGuess where
  constructor Estimate
  actuatorName : String
  actuatorIndex : Fin 102
  activation : ActivationInterval

Observation : Type
Observation = (String, Fin 102, Nat, Nat)

-- Subject L/R (not camera/image L/R). Index order is the 102-actuator
-- native C layout, verified by check_intervals.py against actuators.tsv.
-- Photograph alone cannot establish unique activation or resting tone.
rawObservations : List Observation
rawObservations =
  [
    ("frontalis_medial_L", 0, 45, 67),
    ("frontalis_medial_R", 1, 38, 61),
    ("frontalis_lateral_L", 2, 5, 20),
    ("frontalis_lateral_R", 3, 4, 18),
    ("corrugator_supercilii_L", 6, 28, 50),
    ("corrugator_supercilii_R", 7, 32, 55),
    ("depressor_supercilii_L", 8, 11, 28),
    ("depressor_supercilii_R", 9, 13, 31),
    ("procerus", 10, 14, 34),
    ("orbicularis_oculi_palpebral_upper_L", 11, 6, 20),
    ("orbicularis_oculi_palpebral_upper_R", 12, 8, 22),
    ("orbicularis_oculi_palpebral_lower_L", 13, 6, 19),
    ("orbicularis_oculi_palpebral_lower_R", 14, 8, 22),
    ("orbicularis_oculi_orbital_superior_L", 15, 5, 18),
    ("orbicularis_oculi_orbital_superior_R", 16, 7, 21),
    ("orbicularis_oculi_orbital_inferior_L", 17, 6, 18),
    ("orbicularis_oculi_orbital_inferior_R", 18, 7, 20),
    ("levator_palpebrae_superioris_L", 19, 20, 41),
    ("levator_palpebrae_superioris_R", 20, 17, 38),
    ("orbicularis_oris_upper_medial_L", 40, 7, 19),
    ("orbicularis_oris_upper_medial_R", 41, 6, 18),
    ("orbicularis_oris_lower_medial_L", 44, 6, 17),
    ("orbicularis_oris_lower_medial_R", 45, 6, 16),
    ("depressor_anguli_oris_L", 48, 12, 30),
    ("depressor_anguli_oris_R", 49, 10, 27),
    ("depressor_labii_inferioris_L", 50, 1, 13),
    ("depressor_labii_inferioris_R", 51, 1, 12),
    ("mentalis_L", 52, 3, 15),
    ("mentalis_R", 53, 3, 14)
  ]

toGuess : Observation -> Maybe MuscleGuess
toGuess (name, index, lo, hi) =
  map (Estimate name index) (checkedInterval lo hi)

validatedGuesses : Maybe (List MuscleGuess)
validatedGuesses = traverse toGuess rawObservations

insertMidpoint : Vect 102 Double -> MuscleGuess -> Vect 102 Double
insertMidpoint values guess =
  replaceAt (actuatorIndex guess) (midpoint (activation guess)) values

-- Sparse preview: unspecified entries get ZERO EXTRA MODEL DRIVE. They are
-- unknown physiologically, not claimed relaxed or inactive.
midpointSnapshot : Maybe (Vect 102 Double)
midpointSnapshot =
  map (foldl insertMidpoint (replicate 102 0.0)) validatedGuesses

main : IO ()
main =
  case validatedGuesses of
    Nothing => putStrLn "invalid photo-expression intervals"
    Just gs => putStrLn ("valid interval guesses: " ++ show (length gs))
