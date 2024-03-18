#pragma once

#include "tag-version.h"

enum book_type
{
    BOOK_MINOR_MAGIC,
    BOOK_CONJURATIONS,
    BOOK_FLAMES,
    BOOK_FROST,
    BOOK_WILDERNESS,
    BOOK_FIRE,
    BOOK_ICE,
    BOOK_SPATIAL_TRANSLOCATIONS,
    BOOK_HEXES,
    BOOK_YOUNG_POISONERS,
    BOOK_LIGHTNING,
    BOOK_DEATH,
    BOOK_MISFORTUNE,
    BOOK_SPONTANEOUS_COMBUSTION,
#if TAG_MAJOR_VERSION == 34
    BOOK_TRANSFIGURATIONS,
#endif
    BOOK_FEN,
#if TAG_MAJOR_VERSION == 34
    BOOK_WAR_CHANTS = BOOK_FEN,
#endif
    BOOK_VAPOURS,
    BOOK_NECROMANCY,
    BOOK_CALLINGS,
#if TAG_MAJOR_VERSION == 34
    BOOK_MALEDICT,
#endif
    BOOK_AIR,
#if TAG_MAJOR_VERSION == 34
    BOOK_SKY,
#endif
    BOOK_WARP,
#if TAG_MAJOR_VERSION == 34
    BOOK_ENVENOMATIONS,
#endif
    BOOK_UNLIFE,
#if TAG_MAJOR_VERSION == 34
    BOOK_CONTROL,
#endif
    BOOK_BATTLE,
    BOOK_GEOMANCY,
#if TAG_MAJOR_VERSION == 34
    BOOK_STONE,
    BOOK_WIZARDRY,
#endif
    BOOK_POWER,
    BOOK_CANTRIPS,
    BOOK_PARTY_TRICKS,
#if TAG_MAJOR_VERSION == 34
    BOOK_AKASHIC_RECORD,
#endif
    BOOK_DEBILITATION,
    BOOK_DRAGON,
    BOOK_BURGLARY,
    BOOK_DREAMS,
    BOOK_TRANSMUTATION,
    BOOK_BEASTS,

    BOOK_ANNIHILATIONS,
    BOOK_GRAND_GRIMOIRE,
    BOOK_NECRONOMICON,

    MAX_FIXED_BOOK = BOOK_NECRONOMICON,

    BOOK_RANDART_LEVEL,
    BOOK_RANDART_THEME,

    BOOK_MANUAL,
#if TAG_MAJOR_VERSION == 34
    BOOK_BUGGY_DESTRUCTION,
#endif

    BOOK_SPECTACLE,
    BOOK_WINTER,
    BOOK_SPHERES,
    BOOK_ARMAMENTS,
#if TAG_MAJOR_VERSION == 34
    BOOK_PAIN,
#endif
    BOOK_DECAY,
    BOOK_DISPLACEMENT,
#if TAG_MAJOR_VERSION == 34
    BOOK_RIME,
#endif
    BOOK_EVERBURNING,
    BOOK_EARTH,
#if TAG_MAJOR_VERSION == 34
    BOOK_OZOCUBU,
#endif
    BOOK_SENSES,
    BOOK_MOON,
    BOOK_BLASTING,
    BOOK_IRON,
#if TAG_MAJOR_VERSION == 34
    BOOK_NEARBY,
#endif
    BOOK_TUNDRA,
    BOOK_STORMS,
    BOOK_WEAPONS,
    BOOK_SLOTH,
    BOOK_BLOOD,
    BOOK_THERE_AND_BACK,
    BOOK_DANGEROUS_FRIENDS,
    BOOK_TOUCH,
    BOOK_CHAOS,
    BOOK_UNRESTRAINED,
    BOOK_BIOGRAPHIES_II,
    BOOK_BIOGRAPHIES_VII,
    BOOK_TRISMEGISTUS,
    BOOK_HUNTER,
    BOOK_SCORCHING,
    BOOK_MAXWELL,
    BOOK_MOVEMENT,
    BOOK_WICKED_CREATION,
    NUM_BOOKS
};

#define NUM_FIXED_BOOKS      (MAX_FIXED_BOOK + 1)
