# Build-tool commands at make's fixed interface; owned C has no compiler fallback.
ICK ?= ick
ICK_LINK_FLAGS ?= -fno-link-libatomic
BUILD ?= _/build
CFLAGS ?= -O2
WARN = -std=c11 -Wall -Wextra -Wpedantic -Werror
SOURCE_ROOT = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
MODEL = $(SOURCE_ROOT)/core/tiny_planet.c
HEADER = $(SOURCE_ROOT)/core/tiny_planet.h
REFERENCE_NAMES = -Dtp_walker_init=reference_walker_init -Dtp_walker_set_frame=reference_walker_set_frame -Dtp_walker_turn=reference_walker_turn -Dtp_walker_walk=reference_walker_walk -Dtp_walker_position=reference_walker_position -Dtp_walker_latitude=reference_walker_latitude -Dtp_walker_longitude=reference_walker_longitude -Dtp_vec3_dot=reference_vec3_dot -Dtp_vec3_length=reference_vec3_length

.PHONY: test
test: $(BUILD)/test-tiny-planet $(BUILD)/frame-equivalence
	$(BUILD)/test-tiny-planet
	$(BUILD)/frame-equivalence
$(BUILD):
	mkdir -p "$@"
$(BUILD)/test-tiny-planet: $(MODEL) $(HEADER) $(SOURCE_ROOT)/core/test_tiny_planet.c | $(BUILD)
	$(ICK) $(ICK_LINK_FLAGS) $(WARN) $(CFLAGS) -I"$(SOURCE_ROOT)/core" $(MODEL) $(SOURCE_ROOT)/core/test_tiny_planet.c -lm -o $@
$(BUILD)/reference.o: $(SOURCE_ROOT)/_/tests/reference/tiny_planet.c $(HEADER) | $(BUILD)
	$(ICK) $(WARN) $(CFLAGS) -I"$(SOURCE_ROOT)/core" $(REFERENCE_NAMES) -c $< -o $@
$(BUILD)/frame-equivalence: $(MODEL) $(HEADER) $(SOURCE_ROOT)/_/tests/frame_equivalence.c $(BUILD)/reference.o | $(BUILD)
	$(ICK) $(ICK_LINK_FLAGS) $(WARN) $(CFLAGS) -I"$(SOURCE_ROOT)/core" $(MODEL) $(SOURCE_ROOT)/_/tests/frame_equivalence.c $(BUILD)/reference.o -lm -o $@
