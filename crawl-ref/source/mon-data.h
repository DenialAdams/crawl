#pragma once

#define AT_NO_ATK       {AT_NONE, AF_PLAIN, 0}

#include "enum.h"
#include "tag-version.h"

/* ******************************************************************

   (see "mon-util.h" for the gory details)

 - ordering does not matter, because seekmonster() searches the entire
   array ... probably not to most efficient thing to do, but so it goes

 - Here are the rows:
    - row 1: monster id, display character, display colour, name
    - row 2: monster flags
    - row 3: monster resistance flags
    - row 4: experience modifier, genus, species, holiness, willpower
    - row 5: damage for each of four attacks
    - row 6: hit dice, hit points
    - row 7: AC, evasion, spells, corpse, shouts
    - row 8: intel, habitat, speed, energy_usage
    - row 9: gmon_use class, body size, body shape
    - row 10: tile, corpse

 - Some further explanations:

    - colour: if COLOUR_UNDEF, a random colour will be chosen upon
              creation. Some monsters set their colour during initialization,
              and if a default colour isn't meaningful, they should also use
              COLOUR_UNDEF.
    - name: if an empty string, name generated automagically (see moname)
    - genus: base monster "type" for a classed monsters (i.e. jackal as hound)
    - species: corpse type of monster (i.e. orc for orc wizard)
    - holiness: a bitwise OR of one or more of:
       MH_HOLY       - irritates some gods when killed, immunity from
                        holy wrath weapons. Includes good priests.
       MH_NATURAL    - baseline monster type
       MH_UNDEAD     - immunity from draining, pain, torment; resistance
                        to poison; extra damage from holy wrath;
                        affected by holy word
       MH_DEMONIC    - similar to undead, but no poison resistance
                        *no* automatic damnation resistance
       MH_NONLIVING  - golems and other constructs
       MH_PLANT      - plants

   exp_mod: multiplies xp value after most other calculations.
            see exper_value() in mon-util.cc

   willpower: see mons_willpower() in mon-util.cc
   - If -x calculate (-x * hit dice * 4/3), else simply x.

   damage [4]
   - up to 4 different attacks

   HD: like player level, used for misc things
   avg_hp_10x: average hp for the monster, * 10 for precision
               (see hit_points() for details)

   sec: the monster's spellbook. If a monster has no spells, MST_NO_SPELLS.

   corpse: whether the monster leaves a corpse or not

   shouts
   - various things monsters can do upon seeing you

   intel explanation:
   - How smart it is:
   I_BRAINLESS < I_ANIMAL < I_HUMAN.
   Differences here have a wide variety of small effects; tracking distance,
   behaviour around dangerous clouds, co-operation with allies, etc.

   speed
   - Increases the store of energy that the monster uses for doing things.
   less = slower. 5 = half speed, 10 = normal, 20 = double speed.

   energy usage
   - How quickly the energy granted by speed is used up. Most monsters
   should just use DEFAULT_ENERGY, where all the different types of actions
   use 10 energy units.

   gmon_use explanation:
     MONUSE_NOTHING,
     MONUSE_OPEN_DOORS,
     MONUSE_STARTING_EQUIPMENT,
     MONUSE_WEAPONS_ARMOUR

    From MONUSE_STARTING_EQUIPMENT on, monsters are capable of handling
    items. Contrary to what one might expect, MONUSE_WEAPONS_ARMOUR
    also means a monster is capable of using wands and will also pick
    them up, something that those with MONUSE_STARTING_EQUIPMENT won't
    do.

   size:
     SIZE_TINY,              // rats/bats
     SIZE_LITTLE,            // spriggans
     SIZE_SMALL,             // halflings/kobolds
     SIZE_MEDIUM,            // humans/elves/dwarves
     SIZE_LARGE,             // trolls/ogres/centaurs/nagas
     SIZE_GIANT,             // giants and such

   tile:
    - a struct with up to two elements.
      - the first is the enum for the corresponding sprite in tiles builds;
        TILEP_MONS_PROGRAM_BUG for special cases.
      - the second is the way in which the tile may vary; e.g. over time,
        per-monster-instance, etc.

   corpse:
     - The enum for the corresponding sprite in tiles builds;
       TILE_ERROR for monsters without corpses. Only relevant for species mons.
*/

#define MOVE_ENERGY(x)     { x,  x, 10, 10, 10, 10, 10, 100}
#define ACTION_ENERGY(x)   {10, 10,  x,  x,  x,  x,  x, x * 10}
#define ATTACK_ENERGY(x)   {10, 10,  x, 10, 10, 10, 10, 100}
#define MISSILE_ENERGY(x)  {10, 10, 10,  x, 10, 10, 10, 100}
#define SPELL_ENERGY(x)    {10, 10, 10, 10,  x, 10, 10, 100}
#define SWIM_ENERGY(x)     {10,  x, 10, 10, 10, 10, 10, 100}

#define M_NOT_DANGEROUS (M_NO_EXP_GAIN | M_NO_THREAT)

static monsterentry mondata[] =
{

// The Thing That Should Not Be(tm)
// NOTE: Do not remove, or seekmonster will crash on unknown mc request!
// It is also a good prototype for new monsters.
{
    // id, glyph, colour, name
    MONS_PROGRAM_BUG, 'B', LIGHTRED, "program bug",
    // monster flags
    M_NOT_DANGEROUS | M_CANT_SPAWN,
    // resistance flags
    MR_NO_FLAGS,
    // xp modifier, genus, species, holiness, willpower
    10, MONS_PROGRAM_BUG, MONS_PROGRAM_BUG, MH_NATURAL, 10,
    // up to four attacks
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    // hit points
    0, 0,
    // AC, EV, spells, corpse type, shout type
    0, 0, MST_NO_SPELLS, true, S_SILENT,
    // intelligence, habitat, speed, energy usage
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    // use type, body size, body shape
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

// Use this to replace removed monsters, to retain save compatibility.
// Please put it in #if TAG_MAJOR_VERSION == X, so they will go away
// after save compat is broken.
#define AXED_MON(id, name) \
{ \
    id, 'X', LIGHTRED, "removed " name, \
    M_NO_EXP_GAIN | M_CANT_SPAWN | M_UNFINISHED, \
    MR_NO_FLAGS, \
    10, MONS_PROGRAM_BUG, MONS_PROGRAM_BUG, MH_NONLIVING, 0, \
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK }, \
    0, 0, \
    0, 0, MST_NO_SPELLS, true, S_SILENT, \
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY, \
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC, \
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR \
},

// Axed monsters.
// AXED_MON(MONS_AXE_MURDERER, "Jason")
#if TAG_MAJOR_VERSION == 34
    AXED_MON(MONS_BUMBLEBEE, "bumblebee")
    AXED_MON(MONS_WOOD_GOLEM, "wood golem")
    AXED_MON(MONS_ANT_LARVA, "ant larva")
    AXED_MON(MONS_LABORATORY_RAT, "laboratory rat")
    AXED_MON(MONS_WAR_DOG, "war dog")
    AXED_MON(MONS_SPIRIT, "spirit")
    AXED_MON(MONS_PALADIN, "paladin")
    AXED_MON(MONS_DEEP_ELF_SOLDIER, "deep elf soldier")
    AXED_MON(MONS_PAN, "pan")
    AXED_MON(MONS_LAMIA, "lamia")
    AXED_MON(MONS_DEEP_DWARF_SCION, "deep dwarf scion")
    AXED_MON(MONS_DEEP_DWARF_ARTIFICER, "deep dwarf artificer")
    AXED_MON(MONS_DEEP_DWARF_NECROMANCER, "deep dwarf necromancer")
    AXED_MON(MONS_CHAOS_BUTTERFLY, "chaos butterfly")
    AXED_MON(MONS_POLYMOTH, "polymoth")
    AXED_MON(MONS_MOTH_OF_SUPPRESSION, "moth of suppression")
    AXED_MON(MONS_ROCK_WORM, "rock worm")
    AXED_MON(MONS_FORMICID_DRONE, "formicid drone")
    AXED_MON(MONS_SPIRIT_WOLF, "spirit wolf")
    AXED_MON(MONS_LAVA_FISH, "lava fish")
    AXED_MON(MONS_JELLYFISH, "jellyfish")
    AXED_MON(MONS_GREY_RAT, "grey rat")
    AXED_MON(MONS_SPINY_WORM, "spiny worm")
    AXED_MON(MONS_ROCK_TROLL, "rock troll")
    AXED_MON(MONS_MONSTROUS_ITEM_MIMIC, "monstrous item mimic")
    AXED_MON(MONS_GIANT_AMOEBA, "giant amoeba")
    AXED_MON(MONS_DEEP_DWARF_BERSERKER, "deep dwarf berserker")
    AXED_MON(MONS_AGATE_SNAIL, "agate snail")
    AXED_MON(MONS_GIANT_CENTIPEDE, "giant centipede")
    AXED_MON(MONS_ANCIENT_BEAR, "ancient bear")
    AXED_MON(MONS_ROTTING_DEVIL, "rotting devil")
    AXED_MON(MONS_IGNIS, "ignis")
    AXED_MON(MONS_RAKSHASA_FAKE, "rakshasa fake")
    AXED_MON(MONS_MARA_FAKE, "mara fake")
    // GNOME_NO_MORE
    AXED_MON(MONS_GNOME, "gnome")
    // GOLEM_NO_MORE
    AXED_MON(MONS_CLAY_GOLEM, "clay golem")
    AXED_MON(MONS_STONE_GOLEM, "stone golem")
    AXED_MON(MONS_FORMICID_VENOM_MAGE, "formicid venom mage")
    AXED_MON(MONS_SPRIGGAN_ASSASSIN, "spriggan assassin")
    AXED_MON(MONS_VAPOUR, "vapour")
    AXED_MON(MONS_THORN_LOTUS, "thorn lotus")
    AXED_MON(MONS_GIANT_GOLDFISH, "giant goldfish")
    AXED_MON(MONS_SILVER_STAR, "silver star")
    AXED_MON(MONS_FLAMING_CORPSE, "flaming corpse")
    AXED_MON(MONS_GRIZZLY_BEAR, "grizzly bear")
    AXED_MON(MONS_SPRIGGAN_ENCHANTER, "spriggan enchanter")
    AXED_MON(MONS_PHOENIX, "phoenix")
    AXED_MON(MONS_SHEDU, "shedu")
    AXED_MON(MONS_PLAGUE_SHAMBLER, "plague shambler")
    AXED_MON(MONS_GIANT_SLUG, "giant slug")
    AXED_MON(MONS_FIREFLY, "firefly")
    AXED_MON(MONS_BROWN_OOZE, "brown ooze")
    AXED_MON(MONS_PULSATING_LUMP, "pulsating lump")
    AXED_MON(MONS_BIG_FISH, "big fish")
    AXED_MON(MONS_LAVA_WORM, "lava worm")
    AXED_MON(MONS_SHARK, "shark")
    AXED_MON(MONS_INEPT_ITEM_MIMIC, "inept item mimic")
    AXED_MON(MONS_ITEM_MIMIC, "item mimic")
    AXED_MON(MONS_RAVENOUS_ITEM_MIMIC, "ravenous item mimic")
    AXED_MON(MONS_INEPT_FEATURE_MIMIC, "inept feature mimic")
    AXED_MON(MONS_FEATURE_MIMIC, "feature mimic")
    AXED_MON(MONS_RAVENOUS_FEATURE_MIMIC, "ravenous feature mimic")
    AXED_MON(MONS_MNOLEG_TENTACLE, "mnoleg tentacle")
    AXED_MON(MONS_MNOLEG_TENTACLE_SEGMENT, "mnoleg tentacle segment")
    AXED_MON(MONS_UNBORN, "unborn")
    AXED_MON(MONS_GIANT_MITE, "giant mite")
    AXED_MON(MONS_BABY_ALLIGATOR, "baby alligator")
    AXED_MON(MONS_BORING_BEETLE, "boring beetle")
    AXED_MON(MONS_CHIMERA, "chimera")
    AXED_MON(MONS_RAVEN, "raven")
    AXED_MON(MONS_SINGULARITY, "singularity")
    AXED_MON(MONS_GRAND_AVATAR, "grand avatar")
    AXED_MON(MONS_SALAMANDER_FIREBRAND, "salamander firebrand")
    AXED_MON(MONS_OCTOPODE_CRUSHER, "octopode crusher")
    AXED_MON(MONS_BLUE_DEVIL, "blue devil")
    AXED_MON(MONS_BRAIN_WORM, "brain worm")
    AXED_MON(MONS_DEEP_ELF_SUMMONER, "deep elf summoner")
    AXED_MON(MONS_DEEP_ELF_CONJURER, "deep elf conjurer")
    AXED_MON(MONS_DEEP_ELF_PRIEST, "deep elf priest")
    AXED_MON(MONS_DEEP_ELF_FIGHTER, "deep elf fighter")
    AXED_MON(MONS_TRAPDOOR_SPIDER, "trapdoor spider")
    AXED_MON(MONS_SALAMANDER_STORMCALLER, "salamander stormcaller")
    AXED_MON(MONS_NORRIS, "norris")
    AXED_MON(MONS_MAUD, "maud")
    AXED_MON(MONS_WIGLAF, "wiglaf")
    AXED_MON(MONS_SHEEP, "sheep")
    AXED_MON(MONS_ANUBIS_GUARD, "anubis guard")
    AXED_MON(MONS_GRIFFON, "griffon")
    AXED_MON(MONS_BEETLE, "beetle")
    AXED_MON(MONS_PUTRID_DEMONSPAWN, "putrid demonspawn")
    AXED_MON(MONS_CHAOS_CHAMPION, "chaos champion")
    AXED_MON(MONS_WASP, "wasp")
    AXED_MON(MONS_MOTTLED_DRACONIAN, "mottled draconian")
    AXED_MON(MONS_DRACONIAN_ZEALOT, "draconian zealot")
    AXED_MON(MONS_HILL_GIANT, "hill giant")
    AXED_MON(MONS_BULTUNGIN, "bultungin")
    AXED_MON(MONS_HYPERACTIVE_BALLISTOMYCETE, "hyperactive ballistomycete")
    AXED_MON(MONS_HOMUNCULUS, "homunculus")
    AXED_MON(MONS_SOUPLING, "soupling")
    AXED_MON(MONS_BLESSED_TOE, "blessed toe")
    AXED_MON(MONS_LAVA_ORC, "lava orc")
    AXED_MON(MONS_MONSTROUS_FEATURE_MIMIC, "monstrous feature mimic")
    AXED_MON(MONS_SLAVE, "slave")
    AXED_MON(MONS_HUNGRY_GHOST, "hungry ghost")
    AXED_MON(MONS_CROCODILE, "crocodile")
    AXED_MON(MONS_HIPPOGRIFF, "hippogriff")
    AXED_MON(MONS_PORCUPINE, "porcupine")
    AXED_MON(MONS_CRAWLING_CORPSE, "crawling corpse")
    AXED_MON(MONS_MACABRE_MASS, "macabre mass")
    AXED_MON(MONS_EYE_OF_DRAINING, "eye of draining")
    AXED_MON(MONS_DEATH_OOZE, "death ooze")
    AXED_MON(MONS_LEOPARD_GECKO, "leopard gecko")
    AXED_MON(MONS_WORKER_ANT, "worker ant")
    AXED_MON(MONS_QUEEN_ANT, "queen ant")
    AXED_MON(MONS_SOLDIER_ANT, "soldier ant")
    AXED_MON(MONS_IRONBOUND_BEASTMASTER, "ironbound beastmaster")
    AXED_MON(MONS_MONSTROUS_DEMONSPAWN, "monstrous demonspawn")
    AXED_MON(MONS_GELID_DEMONSPAWN, "gelid demonspawn")
    AXED_MON(MONS_INFERNAL_DEMONSPAWN, "infernal demonspawn")
    AXED_MON(MONS_TORTUROUS_DEMONSPAWN, "torturous demonspawn")
#endif

// Used for genus monsters (which are used for grouping monsters by how they
// work and in comes-into-view messages).
#define DUMMY(id, glyph, colour, name, tile) \
{ \
    (id), (glyph), (colour), (name), \
    M_CANT_SPAWN, \
    MR_NO_FLAGS, \
    10, (id), (id), MH_NONLIVING, 10, \
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK }, \
    0, 0, \
    0, 0, MST_NO_SPELLS, true, S_SILENT, \
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY, \
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC, \
    {(tile)}, TILE_ERROR \
},


// Real monsters begin here {dlb}:

// ancients ('a')
{
    MONS_IMPERIAL_MYRMIDON, 'a', LIGHTCYAN, "imperial myrmidon",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_VULN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 750,
    1, 22, MST_IMPERIAL_MYRMIDON, true, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IMPERIAL_MYRMIDON}, TILE_ERROR
},

{
    MONS_SERVANT_OF_WHISPERS, 'a', BROWN, "servant of whispers",
    M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 800,
    1, 12, MST_SERVANT_OF_WHISPERS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SERVANT_OF_WHISPERS}, TILE_ERROR
},

{
    MONS_RAGGED_HIEROPHANT, 'a', LIGHTMAGENTA, "ragged hierophant",
    M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 1200,
    0, 10, MST_RAGGED_HIEROPHANT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_RAGGED_HIEROPHANT}, TILE_ERROR
},

// batty monsters ('b')
{
    MONS_BAT, 'b', LIGHTGREY, "bat",
    M_SEE_INVIS | M_UNBLINDABLE | M_WARM_BLOOD | M_BATTY | M_FLIES,
    MR_NO_FLAGS,
    4, MONS_BAT, MONS_BAT, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 35,
    1, 14, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_BAT,
    {TILEP_MONS_BAT}, TILE_CORPSE_BAT
},

{
    MONS_BUTTERFLY, 'b', ETC_JEWEL, "butterfly",
    M_CONFUSED | M_NOT_DANGEROUS | M_FLIES,
    MR_VUL_POISON,
    10, MONS_BUTTERFLY, MONS_BUTTERFLY, MH_NATURAL, 10,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 25, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 25, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_BUTTERFLY, TVARY_MOD}, TILE_ERROR
},

{ // one vault + player transform (Vp ability)
    MONS_VAMPIRE_BAT, 'b', MAGENTA, "vampire bat",
    M_SEE_INVIS | M_WARM_BLOOD | M_BATTY | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    8, MONS_BAT, MONS_VAMPIRE_BAT, MH_UNDEAD, 10,
    { {AT_BITE, AF_VAMPIRIC, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 105,
    1, 14, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_BAT,
    {TILEP_MONS_VAMPIRE_BAT}, TILE_ERROR
},

{
    MONS_FIRE_BAT, 'b', LIGHTRED, "fire bat",
    M_SEE_INVIS | M_UNBLINDABLE | M_WARM_BLOOD | M_BATTY | M_FLIES,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    8, MONS_BAT, MONS_FIRE_BAT, MH_NATURAL, 10,
    { {AT_BITE, AF_FIRE, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 225,
    1, 14, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_BAT,
    {TILEP_MONS_FIRE_BAT}, TILE_ERROR
},

{
    MONS_BENNU, 'b', YELLOW, "bennu",
    M_SEE_INVIS | M_WARM_BLOOD | M_FAST_REGEN | M_NO_POLY_TO | M_SPEAKS
        | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_NEG, 3) | MR_RES_POISON | MR_RES_MIASMA,
    6, MONS_BENNU, MONS_BENNU, MH_NATURAL, 140,
    { {AT_PECK, AF_HOLY, 27}, {AT_CLAW, AF_PLAIN, 24},
      {AT_CLAW, AF_PLAIN, 24}, AT_NO_ATK },
    14, 770,
    6, 16, MST_NO_SPELLS, false, S_SCREECH,
    I_HUMAN, HT_LAND, 16, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_BIRD,
    {TILEP_MONS_BENNU}, TILE_ERROR
},

{
    MONS_CAUSTIC_SHRIKE, 'b', LIGHTGREEN, "caustic shrike",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_COLD | MR_RES_FIRE,
    15, MONS_CAUSTIC_SHRIKE, MONS_CAUSTIC_SHRIKE, MH_NATURAL, 80,
    { {AT_CLAW, AF_ACID, 36}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1080,
    8, 18, MST_NO_SPELLS, true, S_SCREECH,
    I_ANIMAL, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_BIRD,
    {TILEP_MONS_CAUSTIC_SHRIKE}, TILE_CORPSE_CAUSTIC_SHRIKE
},

{
    MONS_SHARD_SHRIKE, 'b', LIGHTBLUE, "shard shrike",
    M_WARM_BLOOD | M_BATTY | M_FLIES,
    mrd(MR_RES_COLD, 2),
    12, MONS_SHARD_SHRIKE, MONS_SHARD_SHRIKE, MH_NATURAL, 80,
    { {AT_CLAW, AF_COLD, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    21, 1050,
    2, 18, MST_SHARD_SHRIKE, true, S_SCREECH,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_BIRD,
    {TILEP_MONS_SHARD_SHRIKE}, TILE_CORPSE_SHARD_SHRIKE
},

// centaurs ('c')
{
    MONS_CENTAUR, 'c', BROWN, "centaur",
    M_WARM_BLOOD | M_ARCHER | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_CENTAUR, MONS_CENTAUR, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 7}, {AT_KICK, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK },
    4, 220,
    3, 7, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_CENTAUR,
    {TILEP_MONS_CENTAUR}, TILE_CORPSE_CENTAUR
},

{
    MONS_CENTAUR_WARRIOR, 'c', YELLOW, "centaur warrior",
    M_WARM_BLOOD | M_FIGHTER | M_ARCHER | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_CENTAUR, MONS_CENTAUR, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 16}, {AT_KICK, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    4, 8, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_CENTAUR,
    {TILEP_MONS_CENTAUR_WARRIOR}, TILE_ERROR
},

{
    MONS_YAKTAUR, 'c', RED, "yaktaur",
    M_WARM_BLOOD | M_ARCHER | M_SPEAKS,
    MR_NO_FLAGS,
    9, MONS_YAKTAUR, MONS_YAKTAUR, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    4, 4, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_CENTAUR,
    {TILEP_MONS_YAKTAUR}, TILE_CORPSE_YAKTAUR
},

{
    MONS_YAKTAUR_CAPTAIN, 'c', LIGHTRED, "yaktaur captain",
    M_WARM_BLOOD | M_FIGHTER | M_ARCHER | M_SPEAKS,
    MR_NO_FLAGS,
    9, MONS_YAKTAUR, MONS_YAKTAUR, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    5, 5, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_CENTAUR,
    {TILEP_MONS_YAKTAUR_CAPTAIN}, TILE_ERROR
},

{
    MONS_FAUN, 'c', GREEN, "faun",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    8, MONS_FAUN, MONS_FAUN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 500,
    2, 10, MST_FAUN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_FAUN}, TILE_CORPSE_FAUN
},

{
    MONS_SATYR, 'c', LIGHTGREEN, "satyr",
    M_WARM_BLOOD | M_SPEAKS | M_ARCHER | M_PREFER_RANGED,
    MR_NO_FLAGS,
    10, MONS_FAUN, MONS_SATYR, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    2, 12, MST_SATYR, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_SATYR}, TILE_CORPSE_SATYR
},

// draconians ('d')
{   // Base draconian.
    MONS_DRACONIAN, 'd', BROWN, "draconian",
    M_COLD_BLOOD | M_SPEAKS | M_NO_POLY_TO,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 640,
    10, 11, MST_NO_SPELLS, true, S_ROAR, I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_BASE}, TILE_CORPSE_DRACONIAN_BROWN
},

{
    MONS_BLACK_DRACONIAN, 'd', BLUE, "black draconian",
    M_COLD_BLOOD | M_SPEAKS | M_FLIES,
    mrd(MR_RES_ELEC, 3),
    10, MONS_DRACONIAN, MONS_BLACK_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_BLACK
},

{
    MONS_YELLOW_DRACONIAN, 'd', YELLOW, "yellow draconian",
    M_COLD_BLOOD | M_SPEAKS,
    MR_RES_ACID,
    10, MONS_DRACONIAN, MONS_YELLOW_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_YELLOW
},

{
    // Colours are used for picking the right tile for Tiamat,
    // so this needs to be different from the grey draconian. (jpeg)
    MONS_PALE_DRACONIAN, 'd', CYAN, "pale draconian",
    M_COLD_BLOOD | M_SPEAKS | M_NO_POLY_TO,
    MR_RES_STEAM,
    10, MONS_DRACONIAN, MONS_PALE_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 14, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_PALE
},

{
    MONS_GREEN_DRACONIAN, 'd', GREEN, "green draconian",
    M_COLD_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    10, MONS_DRACONIAN, MONS_GREEN_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, {AT_TAIL_SLAP, AF_POISON, 15}, AT_NO_ATK,
       AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_GREEN
},

{
    MONS_PURPLE_DRACONIAN, 'd', MAGENTA, "purple draconian",
    M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_PURPLE_DRACONIAN, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_PURPLE
},

{
    MONS_RED_DRACONIAN, 'd', LIGHTRED, "red draconian",
    M_COLD_BLOOD | M_SPEAKS,
    MR_RES_FIRE,
    10, MONS_DRACONIAN, MONS_RED_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_RED
},

{
    MONS_WHITE_DRACONIAN, 'd', WHITE, "white draconian",
    M_COLD_BLOOD | M_SPEAKS,
    MR_RES_COLD,
    10, MONS_DRACONIAN, MONS_WHITE_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    9, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_WHITE
},

{
    MONS_GREY_DRACONIAN, 'd', LIGHTGREY, "grey draconian",
    M_COLD_BLOOD | M_SPEAKS | M_NO_POLY_TO,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_GREY_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 25}, {AT_TAIL_SLAP, AF_PLAIN, 15}, AT_NO_ATK,
       AT_NO_ATK },
    14, 980,
    16, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_DRACONIAN_GREY
},

// nonbase draconians ('q')
// Nonbase draconian AC and EV is additive with the base type.
{
    MONS_DRACONIAN_STORMCALLER, 'q', BROWN, "draconian stormcaller",
    M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 880,
    0, 0, MST_DRACONIAN_STORMCALLER, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_STORMCALLER}, TILE_ERROR
},

{
    MONS_DRACONIAN_MONK, 'q', BLUE, "draconian monk",
    M_FIGHTER | M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 35}, {AT_KICK, AF_PLAIN, 20},
      {AT_TAIL_SLAP, AF_PLAIN, 15}, AT_NO_ATK },
    16, 1200,
    -3, 10, MST_NO_SPELLS, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_MONK}, TILE_ERROR
},

{
    MONS_DRACONIAN_SHIFTER, 'q', LIGHTCYAN, "draconian shifter",
    M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 960,
    -1, 6, MST_DRACONIAN_SHIFTER, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_SHIFTER}, TILE_ERROR
},

{
    MONS_DRACONIAN_ANNIHILATOR, 'q', LIGHTBLUE, "draconian annihilator",
    M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 800,
    -1, 0, MST_ANNIHILATOR, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_ANNIHILATOR}, TILE_ERROR
},

{
    MONS_DRACONIAN_KNIGHT, 'q', CYAN, "draconian knight",
    M_FIGHTER | M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1120,
    9, 2, MST_DRACONIAN_KNIGHT, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_KNIGHT}, TILE_ERROR
},

{
    MONS_DRACONIAN_SCORCHER, 'q', LIGHTRED, "draconian scorcher",
    M_COLD_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 800,
    -1, 2, MST_DRACONIAN_SCORCHER, true, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_TAILED,
    {TILEP_DRACO_SCORCHER}, TILE_ERROR
},

// elves ('e')
{
    MONS_ELF, 'e', LIGHTRED, "elf",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 360,
    3, 15, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ELF}, TILE_CORPSE_ELF
},

{
    MONS_DEEP_ELF_KNIGHT, 'e', BLUE, "deep elf knight",
    M_WARM_BLOOD | M_FIGHTER | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 495,
    0, 17, MST_DEEP_ELF_KNIGHT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_KNIGHT}, TILE_ERROR
},

{
    MONS_DEEP_ELF_ARCHER, 'e', BROWN, "deep elf archer",
    M_WARM_BLOOD | M_ARCHER | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 500,
    0, 15, MST_DEEP_ELF_ARCHER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_ARCHER}, TILE_ERROR
},

{
    MONS_DEEP_ELF_BLADEMASTER, 'e', LIGHTCYAN, "deep elf blademaster",
    M_WARM_BLOOD | M_FIGHTER | M_TWO_WEAPONS | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK },
    16, 1040,
    0, 25, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_BLADEMASTER}, TILE_ERROR
},

{
    MONS_DEEP_ELF_MASTER_ARCHER, 'e', LIGHTGREY, "deep elf master archer",
    M_WARM_BLOOD | M_ARCHER | M_PREFER_RANGED | M_SPEAKS,
    MR_NO_FLAGS,
    30, MONS_ELF, MONS_ELF, MH_NATURAL, 100,
    // Attack damage gets rolled into their ranged attacks.
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 750,
    0, 15, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MISSILE_ENERGY(5),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_MASTER_ARCHER}, TILE_ERROR
},

{
    MONS_DEEP_ELF_AIR_MAGE, 'e', CYAN, "deep elf zephyrmancer",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_ELF, MONS_ELF, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 405,
    0, 13, MST_DEEP_ELF_AIR_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_AIR_MAGE}, TILE_ERROR
},

{
    MONS_DEEP_ELF_FIRE_MAGE, 'e', RED, "deep elf pyromancer",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_ELF, MONS_ELF, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 405,
    0, 13, MST_DEEP_ELF_FIRE_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_FIRE_MAGE}, TILE_ERROR
},

{
    MONS_DEEP_ELF_HIGH_PRIEST, 'e', LIGHTGREEN, "deep elf high priest",
    M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    15, MONS_ELF, MONS_ELF, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 495,
    3, 13, MST_DEEP_ELF_HIGH_PRIEST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_HIGH_PRIEST}, TILE_ERROR
},

{
    MONS_DEEP_ELF_DEMONOLOGIST, 'e', YELLOW, "deep elf demonologist",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    20, MONS_ELF, MONS_ELF, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 540,
    0, 13, MST_DEEP_ELF_DEMONOLOGIST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_DEMONOLOGIST}, TILE_ERROR
},

{
    MONS_DEEP_ELF_ANNIHILATOR, 'e', LIGHTBLUE, "deep elf annihilator",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 675,
    0, 13, MST_ANNIHILATOR, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_ANNIHILATOR}, TILE_ERROR
},

{
    MONS_DEEP_ELF_SORCERER, 'e', LIGHTMAGENTA, "deep elf sorcerer",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    17, MONS_ELF, MONS_ELF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 630,
    0, 13, MST_DEEP_ELF_SORCERER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_SORCERER}, TILE_ERROR
},

{
    MONS_DEEP_ELF_DEATH_MAGE, 'e', WHITE, "deep elf death mage",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELF, MONS_ELF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 675,
    0, 13, MST_DEEP_ELF_DEATH_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_DEATH_MAGE}, TILE_ERROR
},

{
    MONS_DEEP_ELF_ELEMENTALIST, 'e', LIGHTRED, "deep elf elementalist",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    17, MONS_ELF, MONS_ELF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 630,
    0, 13, MST_DEEP_ELF_ELEMENTALIST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_ELF_ELEMENTALIST, TVARY_CYCLE}, TILE_ERROR
},

// Mobile plants and fungi ('f')
{
    MONS_WANDERING_MUSHROOM, 'f', BROWN, "wandering mushroom",
    M_NO_FLAGS,
    MR_RES_POISON,
    10, MONS_FUNGUS, MONS_WANDERING_MUSHROOM, MH_PLANT, 40,
    { {AT_SPORE, AF_CONFUSE, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    5, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_FUNGUS,
    {TILEP_MONS_WANDERING_MUSHROOM}, TILE_ERROR
},

{
    MONS_DEATHCAP, 'f', LIGHTMAGENTA, "deathcap",
    M_NO_FLAGS,
    MR_RES_COLD,
    6, MONS_FUNGUS, MONS_WANDERING_MUSHROOM, MH_UNDEAD, 80,
    { {AT_SPORE, AF_CONFUSE, 33}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 520,
    5, 0, MST_DEATHCAP, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_FUNGUS,
    {TILEP_MONS_DEATHCAP}, TILE_ERROR
},

{
    MONS_THORN_HUNTER, 'f', WHITE, "thorn hunter",
    M_SEE_INVIS,
    MR_RES_POISON | MR_VUL_FIRE,
    14, MONS_PLANT, MONS_THORN_HUNTER, MH_PLANT, 100,
    { {AT_HIT, AF_PLAIN, 27}, {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK,
       AT_NO_ATK },
    15, 975,
    9, 9, MST_THORN_HUNTER, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 12, SWIM_ENERGY(12),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_PLANT,
    {TILEP_MONS_THORN_HUNTER}, TILE_ERROR
},

{
    MONS_SHAMBLING_MANGROVE, 'f', LIGHTRED, "shambling mangrove",
    M_NO_FLAGS,
    MR_RES_POISON,
    15, MONS_SHAMBLING_MANGROVE, MONS_SHAMBLING_MANGROVE, MH_PLANT, 100,
    { {AT_HIT, AF_PLAIN, 41}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 910,
    13, 3, MST_SHAMBLING_MANGROVE, false, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS, 8, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_PLANT,
    {TILEP_MONS_TREANT}, TILE_ERROR
},

{
    MONS_VINE_STALKER, 'f', GREEN, "vine stalker",
    M_SPEAKS | M_NO_POLY_TO | M_FAST_REGEN,
    MR_NO_FLAGS,
    10, MONS_VINE_STALKER, MONS_VINE_STALKER, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 10}, {AT_BITE, AF_ANTIMAGIC, 10}, AT_NO_ATK,
       AT_NO_ATK },
    8, 360,
    2, 12, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VINE_STALKER}, TILE_ERROR
},

// goblins and other small humanoids ('g')
{
    MONS_GOBLIN, 'g', LIGHTGREY, "goblin",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_GOBLIN, MONS_GOBLIN, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 40,
    0, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GOBLIN}, TILE_CORPSE_GOBLIN
},

{
    MONS_HOBGOBLIN, 'g', BROWN, "hobgoblin",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_GOBLIN, MONS_HOBGOBLIN, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 55,
    2, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HOBGOBLIN}, TILE_CORPSE_HOBGOBLIN
},

{
    MONS_GNOLL, 'g', YELLOW, "gnoll",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 130,
    2, 9, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GNOLL}, TILE_CORPSE_GNOLL
},

{
    MONS_GNOLL_SHAMAN, 'g', WHITE, "gnoll shaman",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 195,
    2, 9, MST_GNOLL_SHAMAN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GNOLL_SHAMAN}, TILE_ERROR
},

{
    MONS_GNOLL_SERGEANT, 'g', CYAN, "gnoll sergeant",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 11}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 260,
    2, 9, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GNOLL_SERGEANT}, TILE_ERROR
},

{
    MONS_BOGGART, 'g', MAGENTA, "boggart",
    M_SEE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    14, MONS_BOGGART, MONS_BOGGART, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 200,
    0, 12, MST_BOGGART, false, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BOGGART}, TILE_ERROR
},

{ // Another dummy monster.
    MONS_DWARF, 'g', GREEN, "dwarf",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_DWARF, MONS_DWARF, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DWARF}, TILE_CORPSE_DWARF
},

{
    MONS_DEEP_DWARF, 'g', LIGHTBLUE, "deep dwarf",
    M_WARM_BLOOD | M_SPEAKS | M_NO_REGEN | M_NO_POLY_TO,
    MR_NO_FLAGS,
    10, MONS_DWARF, MONS_DEEP_DWARF, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_DWARF}, TILE_CORPSE_DEEP_DWARF
},

// carnivorous quadrupeds ('h')
{
    MONS_JACKAL, 'h', BROWN, "jackal",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_HOUND, MONS_JACKAL, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 55,
    2, 12, MST_NO_SPELLS, true, S_BARK,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_JACKAL}, TILE_CORPSE_JACKAL
},

{
    MONS_HOUND, 'h', YELLOW, "hound",
    M_SEE_INVIS | M_UNBLINDABLE | M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_HOUND, MONS_HOUND, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    2, 13, MST_NO_SPELLS, true, S_BARK,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HOUND}, TILE_CORPSE_HOUND
},

{
    MONS_HOWLER_MONKEY, 'h', LIGHTGREEN, "howler monkey",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    15, MONS_HOWLER_MONKEY, MONS_HOWLER_MONKEY, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 185,
    1, 11, MST_SCREAMER, true, S_HOWL,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_HOWLER_MONKEY}, TILE_CORPSE_HOWLER_MONKEY
},

{
    MONS_WARG, 'h', WHITE, "warg",
    M_SEE_INVIS | M_UNBLINDABLE | M_WARM_BLOOD,
    MR_RES_POISON,
    12, MONS_HOUND, MONS_WARG, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 325,
    4, 12, MST_NO_SPELLS, true, S_HOWL,
    I_ANIMAL, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_WARG}, TILE_CORPSE_WARG
},

{
    MONS_WOLF, 'h', LIGHTGREY, "wolf",
    M_SEE_INVIS | M_UNBLINDABLE | M_WARM_BLOOD,
    MR_NO_FLAGS,
    12, MONS_HOUND, MONS_WOLF, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 220,
    4, 15, MST_NO_SPELLS, true, S_HOWL,
    I_ANIMAL, HT_LAND, 17, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_WOLF}, TILE_CORPSE_WOLF
},

{
    MONS_HOG, 'h', LIGHTMAGENTA, "hog",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_HOG, MONS_HOG, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    2, 9, MST_NO_SPELLS, true, S_SQUEAL,
    I_ANIMAL, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HOG}, TILE_CORPSE_HOG
},

{
    MONS_HELL_HOUND, 'h', CYAN, "hell hound",
    M_SEE_INVIS | M_UNBLINDABLE,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    10, MONS_HOUND, MONS_HELL_HOUND, MH_DEMONIC, 20,
    { {AT_BITE, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    6, 13, MST_HELL_HOUND, true, S_BARK,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HELL_HOUND}, TILE_CORPSE_HELL_HOUND
},

{
    MONS_DOOM_HOUND, 'h', GREEN, "doom hound",
    M_SEE_INVIS | M_UNBLINDABLE,
    MR_NO_FLAGS,
    10, MONS_HOUND, MONS_DOOM_HOUND, MH_DEMONIC, 20,
    { {AT_BITE, AF_PLAIN, 45}, {AT_CLAW, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    20, 1100,
    6, 13, MST_DOOM_HOUND, true, S_BARK,
    I_ANIMAL, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_DOOM_HOUND}, TILE_CORPSE_DOOM_HOUND
},

{
    MONS_RAIJU, 'h', LIGHTCYAN, "raiju",
    M_SEE_INVIS | M_UNBLINDABLE,
    mrd(MR_RES_ELEC, 3),
    12, MONS_HOUND, MONS_RAIJU, MH_NATURAL, 20,
    { {AT_BITE, AF_ELEC, 11}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    4, 14, MST_RAIJU, true, S_BARK,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_RAIJU}, TILE_CORPSE_RAIJU
},

{
    MONS_HELL_HOG, 'h', LIGHTRED, "hell hog",
    M_NO_FLAGS,
    mrd(MR_RES_FIRE, 3),
    10, MONS_HOG, MONS_HELL_HOG, MH_DEMONIC, 40,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 595,
    2, 9, MST_HELL_HOG, true, S_SQUEAL,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HELL_HOG}, TILE_CORPSE_HELL_HOG
},

{   // effect of porkalator cast on holies
    MONS_HOLY_SWINE, 'h', YELLOW, "holy swine",
    M_FLIES,
    MR_NO_FLAGS,
    10, MONS_HOG, MONS_HOLY_SWINE, MH_HOLY, 40,
    { {AT_BITE, AF_HOLY, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 605,
    2, 9, MST_NO_SPELLS, false, S_SQUEAL,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HOLY_SWINE,}, TILE_ERROR
},

{ // a dummy monster for recolouring
    MONS_FELID, 'h', MAGENTA, "felid",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_FELID, MONS_FELID, MH_NATURAL, 40,
    { {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 175,
    2, 18, MST_NO_SPELLS, true, S_HISS,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(8),
    MONUSE_STARTING_EQUIPMENT, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_FELID}, TILE_CORPSE_FELID
},

DUMMY(MONS_BEAR, 'h', LIGHTGREY, "bear", TILEP_MONS_BLACK_BEAR)

{
    MONS_POLAR_BEAR, 'h', LIGHTBLUE, "polar bear",
    M_WARM_BLOOD,
    MR_RES_COLD,
    12, MONS_BEAR, MONS_POLAR_BEAR, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 5}, {AT_CLAW, AF_PLAIN, 5},
       AT_NO_ATK },
    7, 455,
    7, 8, MST_BEAR, true, S_GROWL,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_POLAR_BEAR}, TILE_CORPSE_POLAR_BEAR
},

{
    MONS_BLACK_BEAR, 'h', BLUE, "black bear",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    9, MONS_BEAR, MONS_BLACK_BEAR, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 9}, {AT_CLAW, AF_PLAIN, 5}, {AT_CLAW, AF_PLAIN, 5},
       AT_NO_ATK },
    6, 270,
    2, 8, MST_BEAR, true, S_GROWL,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_BLACK_BEAR}, TILE_CORPSE_BLACK_BEAR
},

// spriggans ('i')
{
    MONS_SPRIGGAN, 'i', LIGHTGREY, "spriggan",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS,
    MR_NO_FLAGS,
    10, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 245,
    1, 18, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPRIGGAN}, TILE_CORPSE_SPRIGGAN
},

{   // both the guy and his ride as one monster
    MONS_SPRIGGAN_RIDER, 'i', LIGHTBLUE, "spriggan rider",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FIGHTER | M_FLIES,
    MR_VUL_POISON, // the mount
    8, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 605,
    1, 18, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID, // ??
    {TILEP_MONS_SPRIGGAN_RIDER}, TILE_ERROR
},

{
    MONS_SPRIGGAN_DRUID, 'i', GREEN, "spriggan druid",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS,
    MR_NO_FLAGS,
    10, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 440,
    1, 18, MST_SPRIGGAN_DRUID, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPRIGGAN_DRUID}, TILE_ERROR
},

{
    MONS_SPRIGGAN_BERSERKER, 'i', LIGHTRED, "spriggan berserker",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FIGHTER,
    MR_NO_FLAGS,
    10, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 540,
    2, 18, MST_SPRIGGAN_BERSERKER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPRIGGAN_BERSERKER}, TILE_ERROR
},

{
    MONS_SPRIGGAN_AIR_MAGE, 'i', LIGHTCYAN, "spriggan air mage",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 490,
    1, 24, MST_SPRIGGAN_AIR_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPRIGGAN_AIR_MAGE}, TILE_ERROR
},

{
    MONS_SPRIGGAN_DEFENDER, 'i', YELLOW, "spriggan defender",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FIGHTER,
    MR_NO_FLAGS,
    10, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 675,
    3, 22, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 16, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPRIGGAN_DEFENDER}, TILE_ERROR
},

// drakes ('k')
DUMMY(MONS_DRAKE, 'k', LIGHTGREY, "drake", TILEP_MONS_SWAMP_DRAKE)

{
    MONS_SWAMP_DRAKE, 'k', BROWN, "swamp drake",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_POISON,
    20, MONS_DRAKE, MONS_SWAMP_DRAKE, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 300,
    3, 11, MST_SWAMP_DRAKE, true, S_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SWAMP_DRAKE}, TILE_CORPSE_SWAMP_DRAKE
},

{
    MONS_RIME_DRAKE, 'k', BLUE, "rime drake",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_COLD,
    22, MONS_DRAKE, MONS_RIME_DRAKE, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    3, 12, MST_RIME_DRAKE, true, S_HISS,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_RIME_DRAKE}, TILE_CORPSE_RIME_DRAKE
},

{
    MONS_WIND_DRAKE, 'k', WHITE, "wind drake",
    M_WARM_BLOOD | M_FLIES,
    MR_NO_FLAGS,
    6, MONS_DRAKE, MONS_WIND_DRAKE, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 600,
    3, 14, MST_WIND_DRAKE, true, S_HISS,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_WIND_DRAKE}, TILE_CORPSE_WIND_DRAKE
},

{
    MONS_LINDWURM, 'k', LIGHTRED, "lindwurm",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    13, MONS_DRAKE, MONS_LINDWURM, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 10},
      {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK },
    9, 495,
    8, 6, MST_LINDWURM, true, S_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_LINDWURM}, TILE_CORPSE_LINDWURM
},

{
    MONS_DEATH_DRAKE, 'k', LIGHTGREY, "death drake",
    M_COLD_BLOOD | M_FLIES,
    MR_RES_POISON | MR_RES_MIASMA,
    10, MONS_DRAKE, MONS_DEATH_DRAKE, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 765,
    6, 14, MST_DEATH_DRAKE, true, S_HISS,
    I_ANIMAL, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_DEATH_DRAKE}, TILE_CORPSE_DEATH_DRAKE
},

// lizards ('l')
DUMMY(MONS_GIANT_LIZARD, 'l', LIGHTGREY, "giant lizard", TILEP_MONS_IGUANA)

{
    MONS_FRILLED_LIZARD, 'l', GREEN, "frilled lizard",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_GIANT_LIZARD, MONS_FRILLED_LIZARD, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 20,
    0, 15, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_FRILLED_LIZARD}, TILE_CORPSE_FRILLED_LIZARD
},

{
    MONS_IGUANA, 'l', BLUE, "iguana",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    13, MONS_GIANT_LIZARD, MONS_IGUANA, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    5, 9, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_IGUANA}, TILE_CORPSE_IGUANA
},

{
    MONS_BASILISK, 'l', MAGENTA, "basilisk",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_GIANT_LIZARD, MONS_BASILISK, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 360,
    3, 12, MST_BASILISK, true, S_HISS,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_BASILISK}, TILE_CORPSE_BASILISK
},

{
    MONS_WYVERN, 'l', LIGHTGREEN, "wyvern",
    M_WARM_BLOOD | M_FLIES,
    MR_NO_FLAGS,
    15, MONS_WYVERN, MONS_WYVERN, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    5, 10, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_WYVERN}, TILE_CORPSE_WYVERN
},

{
    MONS_KOMODO_DRAGON, 'l', LIGHTRED, "komodo dragon",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_GIANT_LIZARD, MONS_KOMODO_DRAGON, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 34}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    7, 8, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_KOMODO_DRAGON}, TILE_CORPSE_KOMODO_DRAGON
},

// merfolk ('m')
{
    MONS_MERFOLK, 'm', LIGHTRED, "merfolk",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    4, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    4, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK, TVARY_WATER}, TILE_CORPSE_MERFOLK
},

{
    MONS_MERFOLK_IMPALER, 'm', YELLOW, "merfolk impaler",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    13, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 780,
    // Impalers prefer light armour, and are dodging experts.
    0, 18, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, {10, 6, 6, 10, 10, 10, 10, 100},
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK_IMPALER, TVARY_WATER}, TILE_ERROR
},

{
    MONS_MERFOLK_JAVELINEER, 'm', WHITE, "merfolk javelineer",
    M_WARM_BLOOD | M_ARCHER | M_PREFER_RANGED | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 730,
    0, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK_JAVELINEER, TVARY_WATER}, TILE_ERROR
},

{
    MONS_MERFOLK_AQUAMANCER, 'm', GREEN, "merfolk aquamancer",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS,
    MR_NO_FLAGS,
    8, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 630,
    0, 12, MST_MERFOLK_AQUAMANCER, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK_AQUAMANCER, TVARY_WATER}, TILE_ERROR
},

{
    MONS_MERFOLK_SIREN, 'm', CYAN, "merfolk siren",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 405,
    4, 12, MST_MERFOLK_SIREN, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK_SIREN, TVARY_WATER}, TILE_ERROR
},

{
    MONS_MERFOLK_AVATAR, 'm', LIGHTCYAN, "merfolk avatar",
    M_WARM_BLOOD | M_SPEAKS | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 120,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 845,
    4, 12, MST_MERFOLK_AVATAR, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MERFOLK_AVATAR, TVARY_WATER}, TILE_ERROR
},

{
    MONS_WATER_NYMPH, 'm', MAGENTA, "water nymph",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_WATER_NYMPH, MONS_WATER_NYMPH, MH_NATURAL, 100,
    { {AT_TOUCH, AF_DROWN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 400,
    2, 13, MST_WATER_NYMPH, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_WATER_NYMPH}, TILE_CORPSE_WATER_NYMPH
},

// undead corpses ('n')
{
    MONS_BOG_BODY, 'n', GREEN, "bog body",
    M_NO_ZOMBIE,
    MR_RES_FIRE | MR_RES_COLD,
    16, MONS_GHOUL, MONS_BOG_BODY, MH_UNDEAD, 40,
    { {AT_TOUCH, AF_COLD, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 390,
    1, 9, MST_BOG_BODY, true, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(14),
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BOG_BODY}, TILE_CORPSE_BOG_BODY
},

{
    MONS_NECROPHAGE, 'n', LIGHTGREY, "necrophage",
    M_NO_ZOMBIE,
    MR_RES_COLD,
    8, MONS_GHOUL, MONS_NECROPHAGE, MH_UNDEAD, 40,
    { {AT_HIT, AF_DRAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    2, 10, MST_NO_SPELLS, true, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_NECROPHAGE}, TILE_CORPSE_NECROPHAGE
},

{
    MONS_BLOATED_HUSK, 'n', YELLOW, "bloated husk",
    M_NO_ZOMBIE,
    MR_RES_COLD,
    10, MONS_GHOUL, MONS_BLOATED_HUSK, MH_UNDEAD, 40,
    {  {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 250,
    5, 5, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS, 14, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BLOATED_HUSK}, TILE_ERROR
},

{
    MONS_GHOUL, 'n', RED, "ghoul",
    M_NO_ZOMBIE,
    MR_RES_COLD,
    10, MONS_GHOUL, MONS_GHOUL, MH_UNDEAD, 80,
    { {AT_CLAW, AF_PLAIN, 30}, {AT_CLAW, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    14, 1470,
    4, 10, MST_NO_SPELLS, true, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GHOUL}, TILE_CORPSE_GHOUL
},

{
    MONS_SEARING_WRETCH, 'n', ETC_FIRE, "searing wretch",
    M_NO_ZOMBIE | M_FAST_REGEN,
    mrd(MR_RES_FIRE, 3) | MR_RES_COLD,
    10, MONS_GHOUL, MONS_SEARING_WRETCH, MH_UNDEAD, 80,
    { {AT_CLAW, AF_FIRE, 40}, {AT_CLAW, AF_SEAR, 30}, {AT_BITE, AF_STICKY_FLAME, 15}, AT_NO_ATK },
    18, 1600,
    4, 8, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_SEARING_WRETCH}, TILE_ERROR
},

// orcs ('o')
{
    MONS_ORC, 'o', LIGHTRED, "orc",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    15, MONS_ORC, MONS_ORC, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 70,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC}, TILE_CORPSE_ORC
},

{
    MONS_ORC_WIZARD, 'o', MAGENTA, "orc wizard",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ORC, MONS_ORC, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 150,
    1, 12, MST_ORC_WIZARD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_WIZARD}, TILE_ERROR
},

{
    MONS_ORC_PRIEST, 'o', GREEN, "orc priest",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ORC, MONS_ORC, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 150,
    1, 10, MST_ORC_PRIEST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_PRIEST}, TILE_ERROR
},

{
    MONS_ORC_WARRIOR, 'o', YELLOW, "orc warrior",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    16, MONS_ORC, MONS_ORC, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 280,
    0, 13, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_WARRIOR}, TILE_ERROR
},

{
    MONS_ORC_KNIGHT, 'o', CYAN, "orc knight",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    9, MONS_ORC, MONS_ORC, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 675,
    2, 13, MST_ORC_KNIGHT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_KNIGHT}, TILE_ERROR
},

{
    MONS_ORC_WARLORD, 'o', LIGHTCYAN, "orc warlord",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    11, MONS_ORC, MONS_ORC, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 1125,
    3, 10, MST_BATTLECRY, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_WARLORD}, TILE_ERROR
},

{
    MONS_ORC_SORCERER, 'o', LIGHTMAGENTA, "orc sorcerer",
    M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    12, MONS_ORC, MONS_ORC, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 315,
    5, 12, MST_ORC_SORCERER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_SORCERER}, TILE_ERROR
},

{
    MONS_ORC_HIGH_PRIEST, 'o', LIGHTGREEN, "orc high priest",
    M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_ORC, MONS_ORC, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 550,
    1, 12, MST_ORC_HIGH_PRIEST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORC_HIGH_PRIEST}, TILE_ERROR
},

// humans ('p')
{
    MONS_HUMAN, 'p', LIGHTGREY, "human",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    3, 13, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HUMAN, TVARY_MOD}, TILE_CORPSE_HUMAN
},

{
    MONS_HELL_KNIGHT, 'p', RED, "hell knight",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    mrd(MR_RES_FIRE, 3),
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 650,
    0, 10, MST_HELL_KNIGHT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HELL_KNIGHT}, TILE_ERROR
},

{
    MONS_DEATH_KNIGHT, 'p', GREEN, "death knight",
    M_WARM_BLOOD | M_FIGHTER | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 28}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 800,
    2, 12, MST_DEATH_KNIGHT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEATH_KNIGHT}, TILE_ERROR
},

{
    MONS_NECROMANCER, 'p', WHITE, "necromancer",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 400,
    0, 13, MST_NECROMANCER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_NECROMANCER}, TILE_ERROR
},

{
    MONS_WIZARD, 'p', MAGENTA, "wizard",
    M_SPEAKS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 400,
    0, 13, MST_WIZARD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_WIZARD}, TILE_ERROR
},

{
    MONS_VAULT_GUARD, 'p', CYAN, "vault guard",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 715,
    1, 13, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAULT_GUARD}, TILE_ERROR
},

{
    MONS_VAULT_SENTINEL, 'p', LIGHTBLUE, "vault sentinel",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 520,
    1, 13, MST_VAULT_SENTINEL, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAULT_SENTINEL}, TILE_ERROR
},

{
    MONS_VAULT_WARDEN, 'p', LIGHTRED, "vault warden",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 36}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 880,
    1, 13, MST_VAULT_WARDEN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAULT_WARDEN}, TILE_ERROR
},

{
    MONS_IRONBOUND_CONVOKER, 'p', YELLOW, "ironbound convoker",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495,
    0, 10, MST_IRONBOUND_CONVOKER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRONBOUND_CONVOKER}, TILE_ERROR
},

{
    MONS_IRONBOUND_FROSTHEART, 'p', BLUE, "ironbound frostheart",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS,
    MR_RES_COLD,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_TOUCH, AF_COLD, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 600,
    0, 10, MST_IRONBOUND_FROSTHEART, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRONBOUND_FROSTHEART}, TILE_ERROR
},

{
    MONS_IRONBOUND_PRESERVER, 'p', LIGHTGREEN, "ironbound preserver",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 1120,
    0, 6, MST_IRONBOUND_PRESERVER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRONBOUND_PRESERVER}, TILE_ERROR
},

{
    MONS_SHAPESHIFTER, 'p', LIGHTRED, "shapeshifter",
    M_NO_SKELETON | M_NO_ZOMBIE,
    MR_NO_FLAGS,
    10, MONS_SHAPESHIFTER, MONS_SHAPESHIFTER, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    0, 10, MST_NO_SPELLS, true, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SHAPESHIFTER}, TILE_CORPSE_SHAPESHIFTER
},

{
    MONS_GLOWING_SHAPESHIFTER, 'p', LIGHTRED, "glowing shapeshifter",
    M_NO_SKELETON | M_NO_ZOMBIE,
    MR_NO_FLAGS,
    10, MONS_SHAPESHIFTER, MONS_GLOWING_SHAPESHIFTER, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    0, 10, MST_NO_SPELLS, true, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GLOWING_SHAPESHIFTER}, TILE_CORPSE_GLOWING_SHAPESHIFTER
},

{
    MONS_KILLER_KLOWN, 'p', ETC_RANDOM, "Killer Klown",
    M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    17, MONS_HUMAN, MONS_KILLER_KLOWN, MH_NATURAL, 160,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1500,
    10, 15, MST_KILLER_KLOWN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KILLER_KLOWN, TVARY_RANDOM}, TILE_CORPSE_KILLER_KLOWN
},

{ // dummy; spawns in a single vault.
    MONS_DEMIGOD, 'p', YELLOW, "demigod",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_DEMIGOD, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 375,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMIGOD}, TILE_CORPSE_HUMAN
},

{ // dummy... literally; single vault
    MONS_HALFLING, 'p', LIGHTGREY, "halfling",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_HALFLING, MONS_HALFLING, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 105,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HALFLING}, TILE_CORPSE_HALFLING
},

{ // 0 XP dummy monster for Okawaru's Arena
    MONS_SPECTATOR, 'p', LIGHTGREY, "spectator",
    M_WARM_BLOOD | M_SPEAKS | M_NOT_DANGEROUS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_SPECTATOR, MH_NATURAL, 20,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1000,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HUMAN, TVARY_MOD}, TILE_CORPSE_HUMAN
},

// rodents ('r')
{
    MONS_RAT, 'r', BROWN, "rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1, MONS_RAT, MONS_RAT, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 25,
    1, 10, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_RAT}, TILE_CORPSE_RAT
},

{
    MONS_QUOKKA, 'r', WHITE, "quokka",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_QUOKKA, MONS_QUOKKA, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 55,
    2, 13, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_QUOKKA}, TILE_CORPSE_QUOKKA
},

{
    MONS_RIVER_RAT, 'r', LIGHTGREEN, "river rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    13, MONS_RAT, MONS_RIVER_RAT, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    5, 11, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_RIVER_RAT}, TILE_CORPSE_GREEN_RAT
},

{
    MONS_HELL_RAT, 'r', LIGHTRED, "hell rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_RAT, MONS_HELL_RAT, MH_DEMONIC, 20,
    { {AT_BITE, AF_DRAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    7, 10, MST_NO_SPELLS, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_ORANGE_RAT}, TILE_CORPSE_ORANGE_RAT
},

{
    MONS_CRYSTAL_ECHIDNA, 'r', GREEN, "crystal echidna",
    M_NO_FLAGS,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    10, MONS_CRYSTAL_ECHIDNA, MONS_CRYSTAL_ECHIDNA, MH_NONLIVING, WILL_INVULN,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 640,
    10, 10, MST_CRYSTAL_ECHIDNA, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_CRYSTAL_ECHIDNA}, TILE_ERROR
},

// arachnids ('s')
{
    MONS_SCORPION, 's', YELLOW, "scorpion",
    M_NO_SKELETON,
    MR_VUL_POISON,
    13, MONS_SCORPION, MONS_SCORPION, MH_NATURAL, 20,
    { {AT_STING, AF_POISON, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 180,
    5, 10, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ARACHNID,
    {TILEP_MONS_SCORPION}, TILE_CORPSE_SCORPION
},

{
    MONS_EMPEROR_SCORPION, 's', LIGHTGREY, "emperor scorpion",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_RES_POISON,
    11, MONS_SCORPION, MONS_EMPEROR_SCORPION, MH_NATURAL, 60,
    { {AT_STING, AF_POISON, 30}, {AT_CLAW, AF_PLAIN, 10},
      {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK },
    14, 1090,
    18, 7, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_ARACHNID,
    {TILEP_MONS_EMPEROR_SCORPION}, TILE_CORPSE_EMPEROR_SCORPION
},

DUMMY(MONS_SPIDER, 's', CYAN, "spider", TILEP_MONS_REDBACK)

{
    MONS_TARANTELLA, 's', LIGHTMAGENTA, "tarantella",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    8, MONS_SPIDER, MONS_TARANTELLA, MH_NATURAL, 20,
    { {AT_TOUCH, AF_CONFUSE, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 280,
    3, 14, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ARACHNID,
    {TILEP_MONS_TARANTELLA}, TILE_CORPSE_TARANTELLA
},

{
    MONS_JUMPING_SPIDER, 's', LIGHTBLUE, "jumping spider",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    6, MONS_SPIDER, MONS_JUMPING_SPIDER, MH_NATURAL, 20,
    { {AT_POUNCE, AF_ENSNARE, 20}, {AT_BITE, AF_PLAIN, 8}, AT_NO_ATK,
       AT_NO_ATK },
    8, 320,
    6, 12, MST_JUMPING_SPIDER, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ARACHNID,
    {TILEP_MONS_JUMPING_SPIDER}, TILE_CORPSE_JUMPING_SPIDER
},

{
    MONS_WOLF_SPIDER, 's', WHITE, "wolf spider",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    4, MONS_SPIDER, MONS_WOLF_SPIDER, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 25}, {AT_BITE, AF_POISON, 15}, AT_NO_ATK,
       AT_NO_ATK },
    11, 550,
    3, 10, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_ARACHNID,
    {TILEP_MONS_WOLF_SPIDER}, TILE_CORPSE_WOLF_SPIDER
},

{
    MONS_REDBACK, 's', LIGHTRED, "redback",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    4, MONS_SPIDER, MONS_REDBACK, MH_NATURAL, 20,
    { {AT_BITE, AF_POISON_STRONG, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 270,
    2, 12, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_ARACHNID,
    {TILEP_MONS_REDBACK}, TILE_CORPSE_REDBACK
},

{
    MONS_DEMONIC_CRAWLER, 's', LIGHTGREEN, "demonic crawler",
    M_NO_SKELETON | M_SEE_INVIS | M_WEB_IMMUNE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD | MR_RES_FIRE,
    4, MONS_DEMONIC_CRAWLER, MONS_DEMONIC_CRAWLER, MH_DEMONIC, 100,
    { {AT_HIT, AF_PLAIN, 16}, {AT_HIT, AF_PLAIN, 16}, {AT_HIT, AF_PLAIN, 16},
       AT_NO_ATK },
    9, 585,
    10, 6, MST_SCREAMER, true, S_SCREAM,
    I_ANIMAL, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_CENTIPEDE,
    {TILEP_MONS_DEMONIC_CRAWLER}, TILE_CORPSE_DEMONIC_CRAWLER
},

{
    MONS_ORB_SPIDER, 's', MAGENTA, "orb spider",
    M_NO_SKELETON | M_MAINTAIN_RANGE | M_WEB_IMMUNE,
    MR_VUL_POISON,
    12, MONS_SPIDER, MONS_ORB_SPIDER, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 420,
    3, 10, MST_ORB_SPIDER, true, S_HISS,
    I_ANIMAL, HT_LAND, 12, SPELL_ENERGY(20),
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ARACHNID,
    {TILEP_MONS_ORB_SPIDER}, TILE_CORPSE_ORB_SPIDER
},

{
    MONS_BROODMOTHER, 's', LIGHTCYAN, "broodmother",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_SPIDER, MONS_BROODMOTHER, MH_NATURAL, 60,
    { {AT_BITE, AF_SPIDER, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 1400,
    2, 4, MST_BROODMOTHER, true, S_HISS,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_ARACHNID,
    {TILEP_MONS_BROODMOTHER}, TILE_CORPSE_BROODMOTHER
},

{
    MONS_CULICIVORA, 's', BROWN, "culicivora",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    5, MONS_SPIDER, MONS_CULICIVORA, MH_NATURAL, 20,
    { {AT_BITE, AF_VAMPIRIC, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 390,
    2, 16, MST_CULICIVORA, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ARACHNID,
    {TILEP_MONS_CULICIVORA}, TILE_CORPSE_CULICIVORA
},

// alligators, testudines, and crabs ('t')
{
    MONS_ALLIGATOR, 't', LIGHTBLUE, "alligator",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_GIANT_LIZARD, MONS_ALLIGATOR, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 30}, {AT_TAIL_SLAP, AF_PLAIN, 15}, AT_NO_ATK,
       AT_NO_ATK },
    12, 720,
    5, 9, MST_ALLIGATOR, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 10, {10, 6, 8, 8, 8, 8, 8, 80},
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_ALLIGATOR}, TILE_CORPSE_ALLIGATOR
},

{
    MONS_SNAPPING_TURTLE, 't', GREEN, "snapping turtle",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_SNAPPING_TURTLE, MONS_SNAPPING_TURTLE, MH_NATURAL, 40,
    { {AT_BITE, AF_REACH, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 600,
    16, 5, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 8, {10, 6, 8, 8, 8, 8, 8, 80},
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_SNAPPING_TURTLE}, TILE_CORPSE_SNAPPING_TURTLE
},

{
    MONS_ALLIGATOR_SNAPPING_TURTLE, 't', LIGHTGREEN, "alligator snapping turtle",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_SNAPPING_TURTLE, MONS_ALLIGATOR_SNAPPING_TURTLE,
        MH_NATURAL, 60,
    { {AT_BITE, AF_REACH, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1440,
    19, 1, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 8, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_ALLIGATOR_SNAPPING_TURTLE}, TILE_CORPSE_ALLIGATOR_SNAPPING_TURTLE
},

DUMMY(MONS_CRAB, 't', LIGHTGREY, "crab", TILEP_MONS_FIRE_CRAB)

{
    MONS_FIRE_CRAB, 't', LIGHTRED, "fire crab",
    M_NO_SKELETON,
    MR_VUL_POISON | mrd(MR_RES_FIRE, 3),
    25, MONS_CRAB, MONS_FIRE_CRAB, MH_NATURAL, 40,
    { {AT_BITE, AF_FIRE, 15}, {AT_CLAW, AF_FIRE, 15}, AT_NO_ATK, AT_NO_ATK },
    8, 520,
    9, 6, MST_FIRE_CRAB, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED, // ??
    {TILEP_MONS_FIRE_CRAB}, TILE_CORPSE_FIRE_CRAB
},

{
    MONS_GHOST_CRAB, 't', LIGHTGREY, "ghost crab",
    M_NO_SKELETON,
    MR_NO_FLAGS,
    25, MONS_CRAB, MONS_GHOST_CRAB, MH_UNDEAD, 40,
    { {AT_BITE, AF_DRAIN, 20}, {AT_CLAW, AF_DRAIN, 15}, AT_NO_ATK, AT_NO_ATK },
    9, 585,
    9, 6, MST_GHOST_CRAB, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_GHOST_CRAB}, TILE_CORPSE_GHOST_CRAB
},

{
    MONS_APOCALYPSE_CRAB, 't', WHITE, "apocalypse crab",
    M_NO_SKELETON | M_SEE_INVIS,
    MR_VUL_POISON | mrd(MR_RES_FIRE | MR_RES_COLD, 2),
    13, MONS_CRAB, MONS_APOCALYPSE_CRAB, MH_DEMONIC, 60,
    { {AT_BITE, AF_CHAOTIC, 15}, {AT_CLAW, AF_CHAOTIC, 15}, AT_NO_ATK, AT_NO_ATK },
    8, 520,
    11, 6, MST_APOCALYPSE_CRAB, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_APOCALYPSE_CRAB}, TILE_CORPSE_APOCALYPSE_CRAB
},

// ugly things ('u')
// NOTE: ugly things are ghost demons and thus their stats vary treacherously
// from those below. beware!
{
    MONS_UGLY_THING, 'u', COLOUR_UNDEF, "ugly thing",
    M_WARM_BLOOD | M_HERD | M_NO_GEN_DERIVED | M_GHOST_DEMON,
    MR_NO_FLAGS,
    6, MONS_UGLY_THING, MONS_UGLY_THING, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    4, 10, MST_NO_SPELLS, true, S_GURGLE,
    I_ANIMAL, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_UGLY_THING
},

{
    MONS_VERY_UGLY_THING, 'u', COLOUR_UNDEF, "very ugly thing",
    M_WARM_BLOOD | M_HERD | M_NO_GEN_DERIVED | M_GHOST_DEMON,
    MR_NO_FLAGS,
    10, MONS_UGLY_THING, MONS_VERY_UGLY_THING, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 990,
    6, 10, MST_NO_SPELLS, true, S_GURGLE,
    I_ANIMAL, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_VERY_UGLY_THING
},

// vortices ('v')
{
    MONS_FIRE_VORTEX, 'v', RED, "fire vortex",
    M_CONFUSED | M_INSUBSTANTIAL | M_CONJURED | M_FLIES | M_NO_EXP_GAIN,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD | MR_RES_ELEC,
    5, MONS_FIRE_VORTEX, MONS_FIRE_VORTEX, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PURE_FIRE, 0}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 300,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_FIRE_VORTEX, TVARY_CYCLE}, TILE_ERROR
},

{
    MONS_SPATIAL_VORTEX, 'v', ETC_RANDOM, "spatial vortex",
    M_CONFUSED | M_INSUBSTANTIAL | M_CONJURED | M_FLIES,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    5, MONS_SPATIAL_VORTEX, MONS_SPATIAL_VORTEX, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_DISTORT, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 540,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_SPATIAL_VORTEX, TVARY_CYCLE}, TILE_ERROR
},

{
    MONS_SPATIAL_MAELSTROM, 'v', YELLOW, "spatial maelstrom",
    M_BATTY | M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC,
    5, MONS_SPATIAL_MAELSTROM, MONS_SPATIAL_MAELSTROM,
        MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_DISTORT, 20}, {AT_HIT, AF_DISTORT, 20}, AT_NO_ATK,
       AT_NO_ATK },
    10, 750,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 16, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_SPATIAL_MAELSTROM, TVARY_CYCLE}, TILE_ERROR
},

// wisp-form
DUMMY(MONS_INSUBSTANTIAL_WISP, 'v', LIGHTGREY, "insubstantial wisp",
      TILEP_MONS_WILL_O_THE_WISP)

{
    MONS_WILL_O_THE_WISP, 'v', GREEN, "will-o-the-wisp",
    M_INSUBSTANTIAL | M_FLIES | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_WILL_O_THE_WISP, MONS_WILL_O_THE_WISP, MH_NONLIVING, WILL_INVULN,
    { {AT_ENGULF, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 400,
    4, 10, MST_WILL_O_THE_WISP, false, S_MOAN,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_MISC,
    {TILEP_MONS_WILL_O_THE_WISP}, TILE_ERROR
},

{ // miscast only
    MONS_TWISTER, 'v', ETC_AIR, "twister",
    M_CONFUSED | M_INSUBSTANTIAL | M_BATTY | M_NO_EXP_GAIN | M_NO_POLY_TO
        | M_FLIES,
    MR_RES_FIRE | mrd(MR_RES_COLD, 3) | mrd(MR_RES_ELEC, 3) | MR_RES_VORTEX,
    5, MONS_FIRE_VORTEX, MONS_TWISTER, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 120000,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_TWISTER, TVARY_CYCLE}, TILE_ERROR
},

// worms and slugs ('w')
{
    MONS_WORM, 'w', LIGHTRED, "worm",
    M_NO_SKELETON | M_FAST_REGEN,
    MR_NO_FLAGS,
    3, MONS_WORM, MONS_WORM, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 180,
    1, 5, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 6, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_SNAKE,
    {TILEP_MONS_WORM}, TILE_CORPSE_WORM
},

{
    MONS_SWAMP_WORM, 'w', BROWN, "swamp worm",
    M_NO_SKELETON,
    MR_NO_FLAGS,
    3, MONS_WORM, MONS_SWAMP_WORM, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 350,
    3, 12, MST_SWAMP_WORM, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 12, MOVE_ENERGY(12),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_SWAMP_WORM, TVARY_WATER}, TILE_CORPSE_SWAMP_WORM
},

{
    MONS_TYRANT_LEECH, 'w', RED, "tyrant leech",
    M_NO_SKELETON,
    MR_NO_FLAGS,
    8, MONS_TYRANT_LEECH, MONS_TYRANT_LEECH, MH_NATURAL, 40,
    { {AT_BITE, AF_VAMPIRIC, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 600,
    5, 15, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 8, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_TYRANT_LEECH}, TILE_CORPSE_TYRANT_LEECH
},

{
    MONS_DART_SLUG, 'w', CYAN, "dart slug",
    M_NO_SKELETON,
    MR_VUL_POISON,
    40, MONS_ELEPHANT_SLUG, MONS_DART_SLUG, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 100,
    1, 1, MST_DART_SLUG, true, S_SILENT,
    I_ANIMAL, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_SNAIL,
    {TILEP_MONS_DART_SLUG}, TILE_CORPSE_DART_SLUG
},

{
    MONS_TORPOR_SNAIL, 'w', GREEN, "torpor snail",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    20, MONS_ELEPHANT_SLUG, MONS_TORPOR_SNAIL, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 600,
    8, 1, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAIL,
    {TILEP_MONS_TORPOR_SNAIL}, TILE_CORPSE_TORPOR_SNAIL
},

{
    MONS_STEELBARB_WORM, 'w', LIGHTGRAY, "steelbarb worm",
    M_NO_SKELETON | M_FAST_REGEN | M_WEB_IMMUNE,
    MR_RES_FIRE,
    8, MONS_WORM, MONS_STEELBARB_WORM, MH_NATURAL, 10,
    { {AT_HIT, AF_BARBS, 29}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 690,
    11, 7, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_SNAKE,
    {TILEP_MONS_STEELBARB_WORM}, TILE_CORPSE_STEELBARB_WORM
},

// dummy genus monster; not using DUMMY since it's also a dummy species
{
    MONS_ELEPHANT_SLUG, 'w', WHITE, "elephant slug",
    M_NO_SKELETON | M_CANT_SPAWN,
    MR_VUL_POISON,
    2, MONS_ELEPHANT_SLUG, MONS_ELEPHANT_SLUG, MH_NATURAL, 80,
    { {AT_BITE, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1300,
    2, 1, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 4, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_SNAIL,
    {TILEP_MONS_DART_SLUG}, TILE_CORPSE_ELEPHANT_SLUG
},

// small abominations ('x')
{
    MONS_UNSEEN_HORROR, 'x', MAGENTA, "unseen horror",
    M_SEE_INVIS | M_INVIS | M_BATTY,
    MR_NO_FLAGS,
    6, MONS_UNSEEN_HORROR, MONS_UNSEEN_HORROR, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    5, 10, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_UNSEEN_HORROR}, TILE_ERROR
},

{
    MONS_ABOMINATION_SMALL, 'x', LIGHTRED, "small abomination",
    M_NO_REGEN,
    MR_NO_FLAGS,
    10, MONS_ABOMINATION_SMALL, MONS_ABOMINATION_SMALL, MH_UNDEAD, 40,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 270,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_ABOMINATION_SMALL, TVARY_MOD}, TILE_ERROR
},

{
    MONS_ANCIENT_ZYME, 'x', GREEN, "ancient zyme",
    M_FLIES,
    MR_NO_FLAGS,
    8, MONS_ANCIENT_ZYME, MONS_ANCIENT_ZYME, MH_NONLIVING, 60,
    { {AT_HIT, AF_PLAIN, 16}, {AT_HIT, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK },
    8, 520,
    6, 6, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_ANCIENT_ZYME}, TILE_ERROR
},

{
    MONS_WORLDBINDER, 'x', CYAN, "worldbinder",
    M_FLIES,
    MR_NO_FLAGS,
    13, MONS_WORLDBINDER, MONS_WORLDBINDER, MH_NONLIVING, 60,
    { {AT_HIT, AF_PLAIN, 8}, {AT_HIT, AF_PLAIN, 8},
       AT_NO_ATK, AT_NO_ATK },
    8, 400,
    12, 4, MST_WORLDBINDER, false, S_SILENT,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_WORLDBINDER}, TILE_ERROR
},

{
    MONS_BUNYIP, 'x', YELLOW, "bunyip",
    M_WARM_BLOOD,
    MR_RES_POISON,
    7, MONS_BUNYIP, MONS_BUNYIP, MH_NATURAL, 60,
    { {AT_CLAW, AF_PLAIN, 40}, {AT_CLAW, AF_PLAIN, 40}, {AT_CLAW, AF_PLAIN, 40}, AT_NO_ATK },
    12, 800,
    6, 10, MST_BUNYIP, true, S_SCREAM,
    I_ANIMAL, HT_AMPHIBIOUS, 15, ATTACK_ENERGY(45),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED /*close enough*/,
    {TILEP_MONS_BUNYIP}, TILE_CORPSE_BUNYIP
},

// flying insects ('y')
{
    MONS_KILLER_BEE, 'y', BROWN, "killer bee",
    M_NO_SKELETON | M_FLIES,
    MR_VUL_POISON,
    11, MONS_KILLER_BEE, MONS_KILLER_BEE, MH_NATURAL, 10,
    { {AT_STING, AF_POISON, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    2, 18, MST_NO_SPELLS, true, S_BUZZ,
    I_ANIMAL, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_KILLER_BEE}, TILE_CORPSE_KILLER_BEE
},

{
    MONS_QUEEN_BEE, 'y', LIGHTMAGENTA, "queen bee",
    M_NO_SKELETON | M_FLIES,
    MR_VUL_POISON,
    14, MONS_KILLER_BEE, MONS_QUEEN_BEE, MH_NATURAL, 20,
    { {AT_STING, AF_POISON, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    10, 10, MST_QUEEN_BEE, true, S_BUZZ,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_QUEEN_BEE}, TILE_CORPSE_QUEEN_BEE
},

{
    MONS_VAMPIRE_MOSQUITO, 'y', LIGHTGREY, "vampire mosquito",
    M_NO_SKELETON | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_VAMPIRE_MOSQUITO, MONS_VAMPIRE_MOSQUITO, MH_UNDEAD, 20,
    { {AT_BITE, AF_VAMPIRIC, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    2, 15, MST_NO_SPELLS, true, S_BUZZ,
    I_ANIMAL, HT_LAND, 19, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_VAMPIRE_MOSQUITO}, TILE_CORPSE_VAMPIRE_MOSQUITO
},

{
    MONS_HORNET, 'y', YELLOW, "hornet",
    M_NO_SKELETON | M_FLIES,
    MR_VUL_POISON,
    13, MONS_HORNET, MONS_HORNET, MH_NATURAL, 30,
    { {AT_STING, AF_POISON_PARALYSE, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    6, 14, MST_NO_SPELLS, true, S_BUZZ,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_HORNET}, TILE_CORPSE_HORNET
},

{
    MONS_MELIAI, 'y', GREEN, "meliai",
    M_NO_SKELETON | M_FLIES | M_WEB_IMMUNE,
    MR_VUL_POISON,
    6, MONS_KILLER_BEE, MONS_MELIAI, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 14}, {AT_STING, AF_POISON, 12}, AT_NO_ATK, AT_NO_ATK },
    7, 245,
    2, 14, MST_MELIAI, true, S_BUZZ,
    I_HUMAN, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_MELIAI}, TILE_CORPSE_MELIAI
},

{
    MONS_SPARK_WASP, 'y', LIGHTCYAN, "spark wasp",
    M_NO_SKELETON | M_FLIES | M_WEB_IMMUNE,
    mrd(MR_RES_ELEC, 3) | MR_VUL_POISON,
    12, MONS_SPARK_WASP, MONS_SPARK_WASP, MH_NATURAL, 40,
    { {AT_STING, AF_ELEC, 33}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    9, 14, MST_SPARK_WASP, true, S_BUZZ,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_SPARK_WASP}, TILE_CORPSE_SPARK_WASP
},

DUMMY(MONS_MOTH, 'y', WHITE, "moth", TILEP_MONS_MOTH_OF_WRATH)

{
    MONS_GHOST_MOTH, 'y', MAGENTA, "ghost moth",
    M_NO_SKELETON | M_INVIS | M_FLIES | M_WEB_IMMUNE,
    MR_RES_POISON | MR_RES_COLD,
    16, MONS_MOTH, MONS_GHOST_MOTH, MH_NATURAL, 100,
    { {AT_HIT, AF_DRAIN_STAT, 24}, {AT_HIT, AF_DRAIN_STAT, 18},
      {AT_STING, AF_DRAIN_STAT, 18}, AT_NO_ATK },
    13, 715,
    8, 10, MST_DRAIN_GAZE, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_GHOST_MOTH}, TILE_CORPSE_GHOST_MOTH
},

{
    MONS_MOTH_OF_WRATH, 'y', LIGHTRED, "moth of wrath",
    M_NO_SKELETON | M_FLIES | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_MOTH, MONS_MOTH_OF_WRATH, MH_NATURAL, 40,
    { {AT_BITE, AF_RAGE, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495,
    0, 10, MST_MOTH_OF_WRATH, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_MOTH_OF_WRATH}, TILE_CORPSE_MOTH_OF_WRATH
},

{
    MONS_SUN_MOTH, 'y', ETC_GOLD, "sun moth",
    M_NO_SKELETON | M_FLIES | M_WEB_IMMUNE,
    MR_RES_FIRE | MR_RES_ELEC,
    10, MONS_MOTH, MONS_SUN_MOTH, MH_HOLY, 60,
    { {AT_BITE, AF_STICKY_FLAME, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 805,
    6, 12, MST_SUN_MOTH, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_INSECT_WINGED,
    {TILEP_MONS_SUN_MOTH}, TILE_CORPSE_SUN_MOTH
},

// corporeal (semi-)humanoid undead ('z')
{
    MONS_WIGHT, 'z', GREEN, "wight",
    M_NO_FLAGS,
    mrd(MR_RES_COLD, 2),
    20, MONS_WIGHT, MONS_WIGHT, MH_UNDEAD, 20,
    { {AT_HIT, AF_DRAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    4, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_WIGHT}, TILE_ERROR
},

{
    MONS_SKELETAL_WARRIOR, 'z', CYAN, "skeletal warrior",
    M_FIGHTER,
    MR_RES_COLD,
    10, MONS_SKELETAL_WARRIOR, MONS_SKELETAL_WARRIOR, MH_UNDEAD, 100,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 650,
    15, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SKELETAL_WARRIOR}, TILE_ERROR
},

{
    MONS_ANCIENT_CHAMPION, 'z', LIGHTCYAN, "ancient champion",
    M_FIGHTER,
    MR_RES_COLD,
    24, MONS_ANCIENT_CHAMPION, MONS_ANCIENT_CHAMPION, MH_UNDEAD, 120,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 700,
    15, 10, MST_ANCIENT_CHAMPION, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCIENT_CHAMPION}, TILE_ERROR
},

{
    MONS_FLYING_SKULL, 'z', WHITE, "flying skull",
    M_NO_FLAGS | M_FLIES, // duh
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_FLYING_SKULL, MONS_FLYING_SKULL, MH_UNDEAD, 20,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 210,
    10, 17, MST_NO_SPELLS, false, S_SCREAM,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_MISC,
    {TILEP_MONS_FLYING_SKULL}, TILE_ERROR
},

{
    MONS_CURSE_SKULL, 'z', LIGHTMAGENTA, "curse skull",
    M_SEE_INVIS | M_SPEAKS | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC,
    20, MONS_CURSE_SKULL, MONS_CURSE_SKULL, MH_UNDEAD, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 520,
    35, 3, MST_CURSE_SKULL, false, S_MOAN,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_MISC,
    {TILEP_MONS_CURSE_SKULL}, TILE_ERROR
},

{
    MONS_CURSE_TOE, 'z', LIGHTGREEN, "curse toe",
    M_SEE_INVIS | M_SPEAKS | M_FLIES,
    MR_RES_ELEC | mrd(MR_RES_FIRE, 3) | MR_RES_COLD,
    60, MONS_CURSE_TOE, MONS_CURSE_TOE, MH_UNDEAD, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 980,
    25, 1, MST_CURSE_TOE, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_MISC,
    {TILEP_MONS_CURSE_TOE}, TILE_ERROR
},

{
    MONS_REVENANT, 'z', MAGENTA, "revenant",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2),
    18, MONS_REVENANT, MONS_REVENANT, MH_UNDEAD, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 810,
    8, 12, MST_REVENANT, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_REVENANT}, TILE_ERROR
},

// angelic beings ('A')
{
    MONS_ANGEL, 'A', WHITE, "angel",
    M_FIGHTER | M_SPEAKS | M_FLIES,
    MR_RES_POISON | MR_RES_ELEC,
    10, MONS_ANGEL, MONS_ANGEL, MH_HOLY, 120,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK,
       AT_NO_ATK },
    12, 1020,
    10, 20, MST_ANGEL, false, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_ANGEL}, TILE_ERROR
},

{
    MONS_CHERUB, 'A', LIGHTBLUE, "cherub",
    M_FIGHTER | M_ARCHER | M_SPEAKS | M_FLIES,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE,
    10, MONS_ANGEL, MONS_CHERUB, MH_HOLY, 100,
    { {AT_HIT, AF_PLAIN, 15}, {AT_CHERUB, AF_PLAIN, 8}, AT_NO_ATK,
       AT_NO_ATK },
    9, 765,
    10, 20, MST_BATTLECRY, false, S_CHERUB,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_CHERUB}, TILE_ERROR
},

{
    MONS_SERAPH, 'A', LIGHTMAGENTA, "seraph",
    M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_FLIES | M_TALL_TILE,
    MR_RES_POISON | MR_RES_ELEC | mrd(MR_RES_FIRE, 3),
    10, MONS_ANGEL, MONS_SERAPH, MH_HOLY, 160,
    { {AT_HIT, AF_PLAIN, 50}, {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK,
       AT_NO_ATK },
    25, 2125,
    10, 20, MST_SERAPH, false, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_SERAPH}, TILE_ERROR
},

{
    MONS_DAEVA, 'A', YELLOW, "daeva",
    M_FIGHTER | M_SPEAKS | M_FLIES,
    MR_RES_POISON,
    12, MONS_ANGEL, MONS_DAEVA, MH_HOLY, 140,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK,
       AT_NO_ATK },
    14, 1190,
    10, 13, MST_DAEVA, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_DAEVA}, TILE_ERROR
},

{
    MONS_PROFANE_SERVITOR, 'A', RED, "profane servitor",
    M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_FLIES,
    MR_RES_COLD | MR_RES_ELEC,
    10, MONS_ANGEL, MONS_PROFANE_SERVITOR, MH_UNDEAD, 140,
    { {AT_HIT, AF_VAMPIRIC, 25}, {AT_HIT, AF_DRAIN, 10}, AT_NO_ATK,
       AT_NO_ATK },
    18, 1530,
    10, 20, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_PROFANE_SERVITOR}, TILE_ERROR
},

// beetles and other insects ('B')

{
    MONS_GIANT_COCKROACH, 'B', BROWN, "giant cockroach",
    M_NO_SKELETON,
    MR_VUL_POISON,
    10, MONS_GIANT_COCKROACH, MONS_GIANT_COCKROACH, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 50,
    3, 10, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_INSECT,
    {TILEP_MONS_GIANT_COCKROACH}, TILE_CORPSE_GIANT_COCKROACH
},

{
    MONS_RADROACH, 'B', LIGHTRED, "radroach",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_GIANT_COCKROACH, MONS_RADROACH, MH_NATURAL, 60,
    { {AT_BITE, AF_PLAIN, 28}, {AT_CLAW, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK },
    12, 800,
    13, 5, MST_RADROACH, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_INSECT,
    {TILEP_MONS_RADROACH}, TILE_CORPSE_RADROACH
},

// dummy for recolouring
{
    MONS_FORMICID, 'B', GREEN, "formicid",
    M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS | M_NO_SKELETON | M_NO_POLY_TO
        | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_FORMICID, MONS_FORMICID, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 420,
    3, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FORMICID}, TILE_CORPSE_FORMICID
},

{
    MONS_ENTROPY_WEAVER, 'B', LIGHTCYAN, "entropy weaver",
    M_WARM_BLOOD | M_NO_SKELETON | M_FIGHTER | M_SEE_INVIS | M_SPEAKS
        | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_ENTROPY_WEAVER, MONS_ENTROPY_WEAVER, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 17} },
    13, 715,
    7, 13, MST_ENTROPY_WEAVER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ENTROPY_WEAVER}, TILE_CORPSE_ENTROPY_WEAVER
},

{
    MONS_BOULDER_BEETLE, 'B', LIGHTGREY, "boulder beetle",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_VUL_POISON,
    12, MONS_BOULDER_BEETLE, MONS_BOULDER_BEETLE, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 765,
    20, 2, MST_BOULDER_BEETLE, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_INSECT,
    {TILEP_MONS_BOULDER_BEETLE}, TILE_CORPSE_BOULDER_BEETLE
},

{
    MONS_PHARAOH_ANT, 'B', YELLOW, "pharaoh ant",
    M_NO_SKELETON | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_PHARAOH_ANT, MONS_PHARAOH_ANT, MH_UNDEAD, 60,
    { {AT_BITE, AF_DRAIN, 43}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 360,
    4, 8, MST_NO_SPELLS, false, S_MOAN,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_INSECT,
    {TILEP_MONS_PHARAOH_ANT}, TILE_ERROR
},


{
    MONS_DEATH_SCARAB, 'B', BLUE, "death scarab",
    M_NO_SKELETON | M_BATTY | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    12, MONS_DEATH_SCARAB, MONS_DEATH_SCARAB, MH_UNDEAD, 60,
    { {AT_BITE, AF_SCARAB, 33}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 320,
    7, 14, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_INSECT,
    {TILEP_MONS_DEATH_SCARAB}, TILE_CORPSE_DEATH_SCARAB
},

// cyclopes and giants ('C')
DUMMY(MONS_GIANT, 'C', LIGHTGREY, "giant", TILEP_MONS_STONE_GIANT)

{
    MONS_CYCLOPS, 'C', YELLOW, "cyclops",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    14, MONS_GIANT, MONS_CYCLOPS, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495,
    5, 3, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CYCLOPS}, TILE_CORPSE_CYCLOPS
},

{
    MONS_ETTIN, 'C', BROWN, "ettin",
    M_WARM_BLOOD | M_TWO_WEAPONS | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_GIANT, MONS_ETTIN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 40}, {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    9, 4, MST_NO_SPELLS, true, S_SHOUT2,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ETTIN}, TILE_CORPSE_ETTIN
},

{
    MONS_FIRE_GIANT, 'C', RED, "fire giant",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_FIRE, 2),
    13, MONS_GIANT, MONS_FIRE_GIANT, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 960,
    8, 4, MST_FIRE_GIANT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FIRE_GIANT}, TILE_CORPSE_FIRE_GIANT
},

{
    MONS_FROST_GIANT, 'C', LIGHTBLUE, "frost giant",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2),
    11, MONS_GIANT, MONS_FROST_GIANT, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1040,
    9, 3, MST_FROST_GIANT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FROST_GIANT}, TILE_CORPSE_FROST_GIANT
},

{
    MONS_STONE_GIANT, 'C', LIGHTGREY, "stone giant",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    9, MONS_GIANT, MONS_STONE_GIANT, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 880,
    12, 2, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_STONE_GIANT}, TILE_CORPSE_STONE_GIANT
},

{
    MONS_TITAN, 'C', LIGHTCYAN, "titan",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_ELEC, 3),
    12, MONS_GIANT, MONS_TITAN, MH_NATURAL, 180,
    { {AT_HIT, AF_PLAIN, 55}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1100,
    10, 3, MST_TITAN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TITAN}, TILE_CORPSE_TITAN
},

{
    MONS_JUGGERNAUT, 'C', LIGHTGREEN, "juggernaut",
    M_FIGHTER | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_GIANT, MONS_JUGGERNAUT, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 80}, {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK },
    20, 1700,
    20, 5, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, ATTACK_ENERGY(45),
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JUGGERNAUT}, TILE_CORPSE_JUGGERNAUT
},

{
    MONS_IRON_GIANT, 'C', CYAN, "iron giant",
    M_WARM_BLOOD | M_SPEAKS | M_FIGHTER | M_SEE_INVIS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    10, MONS_GIANT, MONS_IRON_GIANT, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 65}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1800,
    18, 2, MST_IRON_GIANT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRON_GIANT}, TILE_CORPSE_IRON_GIANT
},

{
    MONS_CACTUS_GIANT, 'C', GREEN, "cactus giant",
    M_SPINY | M_NO_POLY_TO, // summon only
    MR_RES_POISON,
    10, MONS_GIANT, MONS_CACTUS_GIANT, MH_PLANT, 60,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495, // hp will be adjusted later
    1, 2, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(30),
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CACTUS_GIANT}, TILE_ERROR
},

{
    MONS_TAINTED_LEVIATHAN, 'C', MAGENTA, "tainted leviathan",
    M_WARM_BLOOD | M_SPEAKS | M_FIGHTER | M_SEE_INVIS | M_MIASMA_RING,
    MR_RES_COLD,
    10, MONS_GIANT, MONS_TAINTED_LEVIATHAN, MH_UNDEAD, 120,
    { {AT_HIT, AF_PLAIN, 50}, {AT_HIT, AF_DRAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    18, 1500,
    15, 2, MST_TAINTED_LEVIATHAN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TAINTED_LEVIATHAN}, TILE_CORPSE_TAINTED_LEVIATHAN
},

// dragons ('D')
DUMMY(MONS_DRAGON, 'D', GREEN, "dragon", TILEP_MONS_FIRE_DRAGON)

{
    MONS_FIRE_DRAGON, 'D', LIGHTRED, "fire dragon",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2) | MR_VUL_COLD,
    12, MONS_DRAGON, MONS_FIRE_DRAGON, MH_NATURAL, 60,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 13},
      {AT_TRAMPLE, AF_TRAMPLE, 13}, AT_NO_ATK },
    12, 900,
    10, 8, MST_FIRE_DRAGON_BREATH, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_FIRE_DRAGON}, TILE_CORPSE_FIRE_DRAGON
},

{
    MONS_HYDRA, 'D', LIGHTGREEN, "hydra",
    M_COLD_BLOOD | M_FAST_REGEN,
    MR_RES_POISON,
    11, MONS_HYDRA, MONS_HYDRA, MH_NATURAL, 60,
    { {AT_BITE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 715,
    0, 5, MST_NO_SPELLS, true, S_ROAR,
    I_ANIMAL, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_CORPSE_HYDRA
},

{
    MONS_ICE_DRAGON, 'D', WHITE, "ice dragon",
    M_COLD_BLOOD | M_FLIES,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 2),
    10, MONS_DRAGON, MONS_ICE_DRAGON, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 17}, {AT_CLAW, AF_PLAIN, 17},
      {AT_TRAMPLE, AF_TRAMPLE, 17}, AT_NO_ATK },
    12, 900,
    10, 8, MST_ICE_DRAGON_BREATH, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_ICE_DRAGON}, TILE_CORPSE_ICE_DRAGON
},

{
    MONS_STEAM_DRAGON, 'D', BLUE, "steam dragon",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_STEAM,
    29, MONS_DRAGON, MONS_STEAM_DRAGON, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 12}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK },
    4, 300,
    5, 10, MST_STEAM_DRAGON, true, S_HISS, // just for the pun
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_STEAM_DRAGON}, TILE_CORPSE_STEAM_DRAGON
},

{
    MONS_SWAMP_DRAGON, 'D', BROWN, "swamp dragon",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_POISON,
    11, MONS_DRAGON, MONS_SWAMP_DRAGON, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 11},
      {AT_TRAMPLE, AF_TRAMPLE, 11}, AT_NO_ATK },
    9, 675,
    7, 7, MST_SWAMP_DRAGON, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SWAMP_DRAGON}, TILE_CORPSE_SWAMP_DRAGON
},

{
    MONS_ACID_DRAGON, 'D', GREEN, "acid dragon",
    M_WARM_BLOOD | M_FLIES,
    MR_RES_POISON | MR_RES_ACID,
    16, MONS_DRAGON, MONS_ACID_DRAGON, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    5, 10, MST_ACID_DRAGON, true, S_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_ACID_DRAGON}, TILE_CORPSE_ACID_DRAGON
},

{
    MONS_QUICKSILVER_DRAGON, 'D', LIGHTCYAN, "quicksilver dragon",
    M_SEE_INVIS | M_WARM_BLOOD | M_FLIES,
    MR_NO_FLAGS,
    14, MONS_DRAGON, MONS_QUICKSILVER_DRAGON, MH_NATURAL, 140,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    16, 880,
    10, 15, MST_QUICKSILVER_DRAGON, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_QUICKSILVER_DRAGON}, TILE_CORPSE_QUICKSILVER_DRAGON
},

{
    MONS_IRON_DRAGON, 'D', CYAN, "iron dragon",
    M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    14, MONS_DRAGON, MONS_IRON_DRAGON, MH_NATURAL, 160,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 25},
      {AT_TRAMPLE, AF_TRAMPLE, 25}, AT_NO_ATK },
    18, 1170,
    20, 6, MST_SPLINTER_BREATH, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_IRON_DRAGON}, TILE_CORPSE_IRON_DRAGON
},

{
    MONS_STORM_DRAGON, 'D', LIGHTBLUE, "storm dragon",
    M_WARM_BLOOD | M_FLIES,
    mrd(MR_RES_ELEC, 3) | MR_RES_COLD,
    13, MONS_DRAGON, MONS_STORM_DRAGON, MH_NATURAL, 100,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 15},
      {AT_TRAMPLE, AF_TRAMPLE, 15}, AT_NO_ATK },
    14, 1050,
    13, 10, MST_STORM_DRAGON, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_STORM_DRAGON}, TILE_CORPSE_STORM_DRAGON
},

{
    MONS_GOLDEN_DRAGON, 'D', YELLOW, "golden dragon",
    M_SEE_INVIS | M_WARM_BLOOD | M_FLIES,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    26, MONS_DRAGON, MONS_GOLDEN_DRAGON, MH_NATURAL, 180,
    { {AT_BITE, AF_PLAIN, 40}, {AT_CLAW, AF_PLAIN, 20},
      {AT_TRAMPLE, AF_TRAMPLE, 20}, AT_NO_ATK },
    18, 1080,
    15, 7, MST_GOLDEN_DRAGON, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_GOLDEN_DRAGON}, TILE_CORPSE_GOLDEN_DRAGON
},

{
    MONS_SHADOW_DRAGON, 'D', MAGENTA, "shadow dragon",
    M_SEE_INVIS | M_COLD_BLOOD | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | mrd(MR_RES_NEG, 3),
    12, MONS_DRAGON, MONS_SHADOW_DRAGON, MH_NATURAL, 120,
    { {AT_BITE, AF_DRAIN, 20}, {AT_CLAW, AF_PLAIN, 15},
      {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    17, 1275,
    15, 10, MST_SHADOW_DRAGON, true, S_SILENT, // stealth dragon
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SHADOW_DRAGON}, TILE_CORPSE_SHADOW_DRAGON
},

{
    MONS_BONE_DRAGON, 'D', LIGHTGREY, "bone dragon",
    M_SEE_INVIS | M_FLIES,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    12, MONS_DRAGON, MONS_BONE_DRAGON, MH_UNDEAD, 100,
    { {AT_BITE, AF_PLAIN, 30}, {AT_CLAW, AF_PLAIN, 20},
      {AT_TRAMPLE, AF_TRAMPLE, 20}, AT_NO_ATK },
    20, 1800,
    20, 4, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_BONE_DRAGON}, TILE_ERROR
},

{
    MONS_PEARL_DRAGON, 'D', ETC_HOLY, "pearl dragon",
    M_SEE_INVIS | M_WARM_BLOOD | M_FLIES,
    MR_NO_FLAGS,
    16, MONS_DRAGON, MONS_PEARL_DRAGON, MH_HOLY, 160,
    { {AT_BITE, AF_HOLY, 35}, {AT_CLAW, AF_HOLY, 20}, AT_NO_ATK, AT_NO_ATK },
    18, 1170,
    10, 15, MST_PEARL_DRAGON, true, S_LOUD_ROAR,
    I_ANIMAL, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_PEARL_DRAGON}, TILE_CORPSE_PEARL_DRAGON
},

// elementals (E)
DUMMY(MONS_ELEMENTAL, 'E', LIGHTGREY, "elemental", TILEP_MONS_WATER_ELEMENTAL)

{
    MONS_EARTH_ELEMENTAL, 'E', ETC_EARTH, "earth elemental",
    M_NO_FLAGS,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3)
        | MR_RES_PETRIFY,
    13, MONS_ELEMENTAL, MONS_EARTH_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 450,
    14, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 6, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_EARTH_ELEMENTAL}, TILE_ERROR
},

{
    MONS_QUICKSILVER_ELEMENTAL, 'E', LIGHTCYAN, "quicksilver elemental",
    M_SEE_INVIS | M_BATTY,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    10, MONS_ELEMENTAL, MONS_QUICKSILVER_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_ANTIMAGIC, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 800,
    1, 20, MST_QUICKSILVER_DRAGON, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_QUICKSILVER_ELEMENTAL}, TILE_ERROR
},

{
    MONS_FIRE_ELEMENTAL, 'E', ETC_FIRE, "fire elemental",
    M_INSUBSTANTIAL,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD | MR_RES_ELEC,
    10, MONS_ELEMENTAL, MONS_FIRE_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PURE_FIRE, 0}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    4, 12, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS_LAVA, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_FIRE_ELEMENTAL}, TILE_ERROR
},

{
    MONS_WATER_ELEMENTAL, 'E', ETC_WATER, "water elemental",
    M_NO_FLAGS,
    MR_VUL_FIRE | MR_RES_ELEC,
    12, MONS_ELEMENTAL, MONS_WATER_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_ENGULF, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 420,
    4, 7, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_WATER_ELEMENTAL}, TILE_ERROR
},

{
    MONS_AIR_ELEMENTAL, 'E', ETC_AIR, "air elemental",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_ELEC, 3),
    6, MONS_ELEMENTAL, MONS_AIR_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    2, 18, MST_AIR_ELEMENTAL, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 25, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_AIR_ELEMENTAL}, TILE_ERROR
},

{
    MONS_IRON_ELEMENTAL, 'E', ETC_IRON, "iron elemental",
    M_NO_FLAGS,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    13, MONS_ELEMENTAL, MONS_IRON_ELEMENTAL, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 1080,
    20, 2, MST_IRON_ELEMENTAL, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 6, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_IRON_ELEMENTAL}, TILE_ERROR
},

{
    MONS_ELEMENTAL_WELLSPRING, 'E', LIGHTCYAN, "elemental wellspring",
    M_NO_FLAGS,
    MR_RES_ELEC,
    13, MONS_ELEMENTAL, MONS_ELEMENTAL_WELLSPRING, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 620,
    8, 8, MST_ELEMENTAL_WELLSPRING, false, S_SILENT,
    I_BRAINLESS, HT_WATER, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_ELEMENTAL_WELLSPRING}, TILE_ERROR
},

// frogs ('F')
DUMMY(MONS_FROG, 'F', LIGHTGREEN, "giant frog", TILEP_MONS_BULLFROG)

{
    MONS_BULLFROG, 'F', GREEN, "bullfrog",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_FROG, MONS_BULLFROG, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 220,
    0, 12, MST_NO_SPELLS, true, S_CROAK,
    I_ANIMAL, HT_AMPHIBIOUS, 15, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_BULLFROG}, TILE_CORPSE_BULLFROG
},

{
    MONS_CANE_TOAD, 'F', YELLOW, "cane toad",
    M_COLD_BLOOD,
    MR_RES_POISON,
    16, MONS_FROG, MONS_CANE_TOAD, MH_NATURAL, 20,
    { {AT_STING, AF_POISON, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    6, 9, MST_NO_SPELLS, true, S_CROAK,
    I_ANIMAL, HT_AMPHIBIOUS, 12, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_CANE_TOAD}, TILE_CORPSE_CANE_TOAD
},

{
    MONS_BLINK_FROG, 'F', LIGHTGREEN, "blink frog",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    13, MONS_FROG, MONS_BLINK_FROG, MH_NATURAL, 40,
    { {AT_HIT, AF_BLINK, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    0, 16, MST_BLINKER, true, S_CROAK,
    I_ANIMAL, HT_AMPHIBIOUS, 14, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_BLINK_FROG}, TILE_CORPSE_BLINK_FROG
},

{
    MONS_GOLIATH_FROG, 'F', LIGHTGREY, "goliath frog",
    M_COLD_BLOOD,
    MR_NO_FLAGS, // it's tough out there for a frog.
    10, MONS_FROG, MONS_GOLIATH_FROG, MH_NATURAL, 60,
    { { AT_HIT, AF_REACH_TONGUE, 27 }, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 420,
    3, 16, MST_NO_SPELLS, true, S_SILENT, // IRL, goliath frogs have no vocals!
    I_ANIMAL, HT_AMPHIBIOUS, 12, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LITTLE /* ~13" */, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_GOLIATH_FROG}, TILE_CORPSE_GOLIATH_FROG
},

// dummy for recolouring
{
    MONS_BARACHI, 'F', LIGHTCYAN, "barachi",
    M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_FROG, MONS_BARACHI, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 180,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 8, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BULLFROG}, TILE_CORPSE_BULLFROG
},

// eyes ('G')
{
    MONS_FLOATING_EYE, 'G', WHITE, "floating eye",
    M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    6, MONS_FLOATING_EYE, MONS_FLOATING_EYE, MH_NATURAL, 10,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 165,
    0, 1, MST_FLOATING_EYE, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_ORB,
    {TILEP_MONS_FLOATING_EYE}, TILE_ERROR
},

{
    MONS_GLOWING_ORANGE_BRAIN, 'G', LIGHTRED, "glowing orange brain",
    M_WARM_BLOOD | M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    13, MONS_GLOWING_ORANGE_BRAIN, MONS_GLOWING_ORANGE_BRAIN, MH_NATURAL, 100,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    2, 4, MST_GLOWING_ORANGE_BRAIN, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_ORB,
    {TILEP_MONS_GLOWING_ORANGE_BRAIN}, TILE_ERROR
},

{
    MONS_GREAT_ORB_OF_EYES, 'G', LIGHTGREEN, "great orb of eyes",
    M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    13, MONS_FLOATING_EYE, MONS_GREAT_ORB_OF_EYES, MH_NATURAL, 120,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    10, 3, MST_GREAT_ORB_OF_EYES, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_ORB,
    {TILEP_MONS_GREAT_ORB_OF_EYES}, TILE_ERROR
},

{
    MONS_SHINING_EYE, 'G', LIGHTMAGENTA, "shining eye",
    M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    14, MONS_FLOATING_EYE, MONS_SHINING_EYE, MH_NATURAL, 100,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    3, 1, MST_SHINING_EYE, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_ORB,
    {TILEP_MONS_SHINING_EYE}, TILE_ERROR
},

{
    MONS_EYE_OF_DEVASTATION, 'G', YELLOW, "eye of devastation",
    M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    11, MONS_FLOATING_EYE, MONS_EYE_OF_DEVASTATION,
        MH_NATURAL, 100,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    12, 1, MST_EYE_OF_DEVASTATION, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_ORB,
    {TILEP_MONS_EYE_OF_DEVASTATION}, TILE_ERROR
},

{
    MONS_GOLDEN_EYE, 'G', ETC_GOLD, "golden eye",
    M_SEE_INVIS | M_BATTY | M_FLIES,
    MR_NO_FLAGS,
    17, MONS_FLOATING_EYE, MONS_GOLDEN_EYE, MH_NATURAL, 60,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 120,
    0, 20, MST_GOLDEN_EYE, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_ORB,
    {TILEP_MONS_GOLDEN_EYE}, TILE_ERROR
},

{
    MONS_OPHAN, 'G', RED, "ophan",
    M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    14, MONS_ANGEL, MONS_OPHAN, MH_HOLY, 140,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 975,
    10, 10, MST_OPHAN, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_ORB,
    {TILEP_MONS_OPHAN}, TILE_ERROR
},

// hybrids ('H')
{
    MONS_MANTICORE, 'H', RED, "manticore",
    M_WARM_BLOOD | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_MANTICORE, MONS_MANTICORE, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 26}, {AT_CLAW, AF_PLAIN, 14}, {AT_CLAW, AF_PLAIN, 14},
       AT_NO_ATK },
    9, 495,
    5, 7, MST_MANTICORE, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_MANTICORE}, TILE_CORPSE_MANTICORE
},

{
    MONS_MINOTAUR, 'H', LIGHTRED, "minotaur",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_MINOTAUR, MONS_MINOTAUR, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 35}, {AT_GORE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    14, 780,
    6, 9, MST_NO_SPELLS, true, S_BELLOW,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MINOTAUR}, TILE_CORPSE_MINOTAUR
},

{
    MONS_FENSTRIDER_WITCH, 'H', LIGHTMAGENTA, "fenstrider witch",
    M_SPEAKS | M_WARM_BLOOD | M_SEE_INVIS,
    MR_RES_POISON | MR_RES_NEG,
    10, MONS_FENSTRIDER_WITCH, MONS_FENSTRIDER_WITCH, MH_NATURAL, 100,
    { { AT_HIT, AF_PLAIN, 25 }, { AT_KICK, AF_PLAIN, 25 }, AT_NO_ATK, AT_NO_ATK },
    16, 666,
    3, 15, MST_FENSTRIDER_WITCH, true, S_SHOUT,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_GIANT, MON_SHAPE_HUMANOID,
    { TILEP_MONS_FENSTRIDER_WITCH }, TILE_CORPSE_FENSTRIDER_WITCH
},

{
    MONS_SPHINX, 'H', LIGHTGREY, "sphinx",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    12, MONS_SPHINX, MONS_SPHINX, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12},
       AT_NO_ATK },
    16, 880,
    5, 5, MST_SPHINX, true, S_SHOUT,
    I_HUMAN, HT_LAND, 11, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SPHINX}, TILE_CORPSE_SPHINX
},

{
    MONS_HARPY, 'H', GREEN, "harpy",
    M_WARM_BLOOD | M_BATTY | M_FLIES,
    MR_RES_POISON,
    9, MONS_HARPY, MONS_HARPY, MH_NATURAL, 20,
    { {AT_CLAW, AF_PLAIN, 19}, {AT_CLAW, AF_PLAIN, 14}, AT_NO_ATK,
       AT_NO_ATK },
    7, 385,
    2, 10, MST_NO_SPELLS, true, S_SCREECH,
    I_HUMAN, HT_LAND, 25, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_HARPY}, TILE_CORPSE_HARPY
},

{
    MONS_JOROGUMO, 'H', BLUE, "jorogumo",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_WEB_IMMUNE,
    MR_NO_FLAGS,
    10, MONS_JOROGUMO, MONS_JOROGUMO, MH_NATURAL, 60,
    { {AT_HIT, AF_POISON_PARALYSE, 28}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 850,
    4, 14, MST_JOROGUMO, true, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_ARACHNID,
    {TILEP_MONS_JOROGUMO}, TILE_CORPSE_JOROGUMO
},

// dummy, though it's rude to say so
{
    MONS_PALENTONGA, 'H', YELLOW, "palentonga",
    M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_PALENTONGA, MONS_PALENTONGA, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 18}, {AT_TAIL_SLAP, AF_PLAIN, 12}, AT_NO_ATK,
        AT_NO_ATK },
    8, 450,
    15, 5, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BOULDER_BEETLE}, TILE_CORPSE_BOULDER_BEETLE
},

// Mutant beasts - variable traits
{
    MONS_MUTANT_BEAST, 'H', MAGENTA, "mutant beast",
    M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    11, MONS_MUTANT_BEAST, MONS_MUTANT_BEAST, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 825,
    8, 5, MST_NO_SPELLS, true, S_RANDOM,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED,
    {TILEP_MUTANT_BEAST_BASE}, TILE_CORPSE_MUTANT_BEAST
},


// Hepliaklqana's ancestor - variable traits
{
    MONS_ANCESTOR, 'R', LIGHTCYAN, "ancestor",
    M_ANCESTOR | M_NO_POLY_TO | M_NO_GEN_DERIVED | M_FLIES | M_FAST_REGEN | M_INSUBSTANTIAL | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD,
    1, MONS_ANCESTOR, MONS_ANCESTOR, MH_NONLIVING, -5,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 1,
    5, 10, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCESTOR}, TILE_ERROR
},

{
    MONS_ANCESTOR_KNIGHT, 'R', LIGHTCYAN, "knight",
    M_ANCESTOR | M_NO_POLY_TO | M_NO_GEN_DERIVED | M_FLIES | M_FAST_REGEN | M_INSUBSTANTIAL | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD,
    1, MONS_ANCESTOR, MONS_ANCESTOR_KNIGHT, MH_NONLIVING, -5,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 1,
    5, 10, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCESTOR_KNIGHT}, TILE_ERROR
},
{
    MONS_ANCESTOR_BATTLEMAGE, 'R', LIGHTCYAN, "battlemage",
    M_ANCESTOR | M_NO_POLY_TO | M_NO_GEN_DERIVED | M_FLIES | M_FAST_REGEN | M_INSUBSTANTIAL | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD,
    1, MONS_ANCESTOR, MONS_ANCESTOR_BATTLEMAGE, MH_NONLIVING, -5,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 1,
    5, 10, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCESTOR_BATTLEMAGE}, TILE_ERROR
},
{
    MONS_ANCESTOR_HEXER, 'R', LIGHTCYAN, "hexer",
    M_ANCESTOR | M_NO_POLY_TO | M_NO_GEN_DERIVED | M_FLIES | M_FAST_REGEN | M_INSUBSTANTIAL | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD,
    1, MONS_ANCESTOR, MONS_ANCESTOR_HEXER, MH_NONLIVING, -5,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 1,
    5, 10, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(5),
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCESTOR_HEXER}, TILE_ERROR
    },

// jellies ('J')
{
    MONS_ENDOPLASM, 'J', LIGHTGREY, "endoplasm",
    M_SEE_INVIS | M_UNBLINDABLE | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    10, MONS_JELLY, MONS_ENDOPLASM, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 60,
    1, 3, MST_ENDOPLASM, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_BLOB,
    {TILEP_MONS_ENDOPLASM}, TILE_ERROR
},

{
    MONS_JELLY, 'J', LIGHTRED, "jelly",
    M_SEE_INVIS | M_UNBLINDABLE | M_SPLITS | M_ACID_SPLASH | M_EAT_DOORS
        | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    15, MONS_JELLY, MONS_JELLY, MH_NATURAL, 10,
    { {AT_HIT, AF_ACID, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    0, 2, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_BLOB,
    {TILEP_MONS_JELLY}, TILE_ERROR
},

{
    MONS_SLIME_CREATURE, 'J', GREEN, "slime creature",
    M_FAST_REGEN | M_HERD | M_WEB_IMMUNE,
    MR_RES_POISON,
    3, MONS_JELLY, MONS_SLIME_CREATURE, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 605,
    1, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_BLOB,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

DUMMY(MONS_MERGED_SLIME_CREATURE, 'J', LIGHTGREEN, "merged slime creature",
      TILEP_MONS_PROGRAM_BUG)

{
    MONS_ROCKSLIME, 'J', BROWN, "rockslime",
    M_SEE_INVIS | M_UNBLINDABLE | M_EAT_DOORS | M_WEB_IMMUNE,
    MR_RES_POISON | MR_RES_ELEC | mrd(MR_RES_FIRE, 2),
    10, MONS_JELLY, MONS_ROCKSLIME, MH_NATURAL, 60,
    { {AT_HIT, AF_TRAMPLE, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 600,
    27, 2, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_BLOB,
    {TILEP_MONS_ROCKSLIME}, TILE_ERROR
},

{
    MONS_QUICKSILVER_OOZE, 'J', CYAN, "quicksilver ooze",
    M_SEE_INVIS | M_UNBLINDABLE | M_EAT_DOORS | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    15, MONS_JELLY, MONS_QUICKSILVER_OOZE, MH_NATURAL, WILL_INVULN,
    { {AT_HIT, AF_ENGULF, 27}, {AT_HIT, AF_ANTIMAGIC, 15}, AT_NO_ATK, AT_NO_ATK },
    13, 500,
    3, 15, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 16, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_BLOB,
    {TILEP_MONS_QUICKSILVER_OOZE}, TILE_ERROR
},

{
    MONS_AZURE_JELLY, 'J', LIGHTBLUE, "azure jelly",
    M_SEE_INVIS | M_UNBLINDABLE | M_EAT_DOORS | M_WEB_IMMUNE,
    MR_RES_POISON | MR_RES_COLD | MR_VUL_FIRE | mrd(MR_RES_ACID, 3),
    14, MONS_JELLY, MONS_AZURE_JELLY, MH_NATURAL, 80,
    { {AT_HIT, AF_COLD, 12}, {AT_HIT, AF_COLD, 12}, {AT_HIT, AF_PLAIN, 12},
      {AT_HIT, AF_PLAIN, 12} },
    15, 825,
    5, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_BLOB,
    {TILEP_MONS_AZURE_JELLY}, TILE_ERROR
},

{
    MONS_ACID_BLOB, 'J', LIGHTCYAN, "acid blob",
    M_SEE_INVIS | M_UNBLINDABLE | M_ACID_SPLASH | M_EAT_DOORS | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    12, MONS_JELLY, MONS_ACID_BLOB, MH_NATURAL, 160,
    { {AT_HIT, AF_ACID, 42}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 990,
    1, 3, MST_ACID_SPIT, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_BLOB,
    {TILEP_MONS_ACID_BLOB}, TILE_ERROR
},

// kobolds ('K')
{
    MONS_KOBOLD, 'K', BROWN, "kobold",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, 0,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 35,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KOBOLD}, TILE_CORPSE_KOBOLD
},

{
    MONS_KOBOLD_BRIGAND, 'K', YELLOW, "kobold brigand",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    7, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    3, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KOBOLD_BRIGAND}, TILE_ERROR
},

{
    MONS_KOBOLD_DEMONOLOGIST, 'K', MAGENTA, "kobold demonologist",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    18, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 390,
    2, 13, MST_KOBOLD_DEMONOLOGIST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KOBOLD_DEMONOLOGIST}, TILE_ERROR
},

// liches ('L')
{
    MONS_LICH, 'L', LIGHTGREY, "lich",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2),
    18, MONS_LICH, MONS_LICH, MH_UNDEAD, WILL_INVULN,
    { {AT_TOUCH, AF_DRAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 800,
    10, 10, MST_LICH, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LICH}, TILE_ERROR
},

{
    MONS_ANCIENT_LICH, 'L', WHITE, "ancient lich",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2) | MR_RES_FIRE | MR_RES_ELEC,
    24, MONS_LICH, MONS_LICH, MH_UNDEAD, WILL_INVULN,
    { {AT_TOUCH, AF_DRAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    27, 1080,
    20, 10, MST_ANCIENT_LICH, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANCIENT_LICH}, TILE_ERROR
},

{
    MONS_DREAD_LICH, 'L', LIGHTMAGENTA, "dread lich",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2) | MR_RES_FIRE | MR_RES_ELEC,
    24, MONS_LICH, MONS_LICH, MH_UNDEAD, WILL_INVULN,
    { {AT_TOUCH, AF_DRAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    27, 1080,
    20, 10, MST_DREAD_LICH, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DREAD_LICH}, TILE_ERROR
},

{
    MONS_HALAZID_WARLOCK, 'L', GREEN, "halazid warlock",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_COLD, 2),
    10, MONS_LICH, MONS_LICH, MH_UNDEAD, 100,
    { {AT_TOUCH, AF_PAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 750,
    8, 12, MST_HALAZID_WARLOCK, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HALAZID_WARLOCK}, TILE_ERROR
},

{
    MONS_STOKER, 'L', ETC_FIRE, "stoker",
    M_NO_SKELETON | M_MAINTAIN_RANGE,
    mrd(MR_RES_FIRE, 3),
    10, MONS_LICH, MONS_STOKER, MH_UNDEAD, 120,
    { {AT_TOUCH, AF_FIRE, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 800,
    5, 12, MST_STOKER, false, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS_LAVA, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_STOKER}, TILE_ERROR
},

// mummies ('M')
{
    MONS_MUMMY, 'M', LIGHTGREY, "mummy",
    M_NO_FLAGS,
    MR_VUL_FIRE | MR_RES_COLD,
    21, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 20,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 195,
    3, 6, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 6, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MUMMY}, TILE_ERROR
},

{
    MONS_GUARDIAN_MUMMY, 'M', YELLOW, "guardian mummy",
    M_FIGHTER,
    MR_RES_COLD,
    13, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 40,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 455,
    6, 9, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GUARDIAN_MUMMY}, TILE_ERROR
},

{
    MONS_ROYAL_MUMMY, 'M', WHITE, "royal mummy",
    M_SEE_INVIS | M_SPEAKS,
    MR_RES_COLD | MR_RES_ELEC,
    24, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 160,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 1950,
    10, 6, MST_ROYAL_MUMMY, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ROYAL_MUMMY}, TILE_ERROR
},

{
    MONS_MUMMY_PRIEST, 'M', RED, "mummy priest",
    M_SPEAKS,
    MR_RES_COLD | MR_RES_ELEC,
    20, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 120,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 650,
    8, 7, MST_MUMMY_PRIEST, false, S_SILENT,
    I_HUMAN, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MUMMY_PRIEST}, TILE_ERROR
},

// nagas ('N')
{
    MONS_NAGA, 'N', GREEN, "naga",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    13, MONS_NAGA, MONS_NAGA, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 22}, {AT_CONSTRICT, AF_CRUSH, 5},
       AT_NO_ATK, AT_NO_ATK },
    5, 275,
    6, 10, MST_NAGA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGA}, TILE_CORPSE_NAGA
},

{
    MONS_NAGA_MAGE, 'N', MAGENTA, "naga mage",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    14, MONS_NAGA, MONS_NAGA, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 14}, {AT_CONSTRICT, AF_CRUSH, 4},
       AT_NO_ATK, AT_NO_ATK },
    7, 385,
    6, 10, MST_NAGA_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGA_MAGE}, TILE_ERROR
},

{
    MONS_NAGA_SHARPSHOOTER, 'N', LIGHTGRAY, "naga sharpshooter",
    M_SEE_INVIS | M_WARM_BLOOD
        | M_ARCHER | M_PREFER_RANGED | M_SPEAKS,
    MR_RES_POISON,
    14, MONS_NAGA, MONS_NAGA, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 17}, {AT_CONSTRICT, AF_CRUSH, 5}, AT_NO_ATK, AT_NO_ATK },
    9, 720,
    6, 10, MST_NAGA_SHARPSHOOTER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGA_SHARPSHOOTER}, TILE_ERROR
},

{
    MONS_NAGA_RITUALIST, 'N', BROWN, "naga ritualist",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    14, MONS_NAGA, MONS_NAGA, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 14}, {AT_CONSTRICT, AF_CRUSH, 4},
       AT_NO_ATK, AT_NO_ATK },
    8, 600,
    6, 10, MST_NAGA_RITUALIST, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGA_RITUALIST}, TILE_ERROR
},

{
    MONS_NAGA_WARRIOR, 'N', BLUE, "naga warrior",
    M_FIGHTER | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    11, MONS_NAGA, MONS_NAGA, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 32}, {AT_CONSTRICT, AF_CRUSH, 6},
       AT_NO_ATK, AT_NO_ATK },
    10, 800,
    6, 10, MST_NAGA_WARRIOR, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGA_WARRIOR}, TILE_ERROR
},

{
    MONS_NAGARAJA, 'N', LIGHTMAGENTA, "nagaraja",
    M_FIGHTER | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    15, MONS_NAGA, MONS_NAGA, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 27}, {AT_CONSTRICT, AF_CRUSH, 7},
       AT_NO_ATK, AT_NO_ATK },
    15, 825,
    6, 10, MST_NAGARAJA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_NAGARAJA}, TILE_ERROR
},

// ogres ('O')
{
    MONS_OGRE, 'O', BROWN, "ogre",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    9, MONS_OGRE, MONS_OGRE, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    1, 6, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_OGRE}, TILE_CORPSE_OGRE
},

// These guys get understated because the experience code can't see
// that they wield two weapons... I'm raising their xp modifier. - bwr
{
    MONS_TWO_HEADED_OGRE, 'O', LIGHTRED, "two-headed ogre",
    M_WARM_BLOOD | M_TWO_WEAPONS | M_SPEAKS,
    MR_NO_FLAGS,
    15, MONS_OGRE, MONS_TWO_HEADED_OGRE, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 24}, {AT_HIT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    3, 6, MST_NO_SPELLS, true, S_SHOUT2,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TWO_HEADED_OGRE}, TILE_CORPSE_TWO_HEADED_OGRE
},

{
    MONS_OGRE_MAGE, 'O', MAGENTA, "ogre mage",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    16, MONS_OGRE, MONS_OGRE, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    1, 7, MST_OGRE_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_OGRE_MAGE}, TILE_ERROR
},

{
    MONS_IRONBOUND_THUNDERHULK, 'O', LIGHTCYAN, "ironbound thunderhulk",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    7, MONS_OGRE, MONS_OGRE, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 880,
    1, 8, MST_IRONBOUND_THUNDERHULK, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRONBOUND_THUNDERHULK}, TILE_ERROR
},

// immobile plants and fungi ('P')
{
    MONS_PLANT, 'P', GREEN, "plant",
    M_STATIONARY | M_NOT_DANGEROUS | M_FRAGILE,
    MR_RES_POISON,
    10, MONS_PLANT, MONS_PLANT, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 2000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_PLANT, TVARY_MOD}, TILE_ERROR
},

{
    MONS_DEMONIC_PLANT, 'P', ETC_RANDOM, "demonic plant",
    M_STATIONARY | M_NOT_DANGEROUS | M_FRAGILE,
    MR_RES_POISON,
    10, MONS_PLANT, MONS_DEMONIC_PLANT, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 2000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_DEMONIC_PLANT}, TILE_ERROR
},

{
    MONS_WITHERED_PLANT, 'P', DARKGREY, "withered plant",
    M_STATIONARY | M_NOT_DANGEROUS | M_FRAGILE,
    mrd(MR_RES_NEG | MR_RES_POISON, 3),
    10, MONS_PLANT, MONS_WITHERED_PLANT, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 2000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_WITHERED_PLANT}, TILE_ERROR
},

{
    MONS_OKLOB_SAPLING, 'P', LIGHTCYAN, "oklob sapling",
    M_STATIONARY,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    10, MONS_PLANT, MONS_OKLOB_PLANT, MH_PLANT, 20,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 220,
    10, 0, MST_OKLOB_SAPLING, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_OKLOB_SAPLING}, TILE_ERROR
},

{
    MONS_OKLOB_PLANT, 'P', LIGHTGREEN, "oklob plant",
    M_STATIONARY,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    10, MONS_PLANT, MONS_OKLOB_PLANT, MH_PLANT, 40,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    10, 0, MST_ACID_SPIT, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_OKLOB_PLANT}, TILE_ERROR
},

{
    MONS_BUSH, 'P', BROWN, "bush",
    M_STATIONARY | M_NOT_DANGEROUS,
    MR_RES_POISON | MR_VUL_FIRE,
    10, MONS_PLANT, MONS_BUSH, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1100,
    15, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_PLANT,
    {TILEP_MONS_BUSH, TVARY_MOD}, TILE_ERROR
},

{
    MONS_BURNING_BUSH, 'P', RED, "burning bush",
    M_STATIONARY | M_SEE_INVIS,
    MR_RES_POISON | MR_RES_FIRE,
    10, MONS_PLANT, MONS_BUSH, MH_PLANT, 40,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    10, 0, MST_BURNING_BUSH, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_PLANT,
    {TILEP_MONS_BUSH_BURNING}, TILE_ERROR
},

{
    MONS_TOADSTOOL, 'P', COLOUR_UNDEF, "toadstool",
    M_NOT_DANGEROUS | M_STATIONARY,
    MR_RES_POISON,
    10, MONS_FUNGUS, MONS_TOADSTOOL, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 30,
    1, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_FUNGUS,
    {TILEP_MONS_TOADSTOOL, TVARY_MOD}, TILE_ERROR
},

{
    MONS_FUNGUS, 'P', LIGHTGREY, "fungus",
    M_NOT_DANGEROUS | M_STATIONARY | M_FRAGILE,
    MR_RES_POISON,
    10, MONS_FUNGUS, MONS_FUNGUS, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 2000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_FUNGUS,
    {TILEP_MONS_FUNGUS, TVARY_MOD}, TILE_ERROR
},

{
    MONS_BALLISTOMYCETE, 'P', MAGENTA, "ballistomycete",
    M_STATIONARY,
    MR_RES_POISON,
    10, MONS_FUNGUS, MONS_BALLISTOMYCETE, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 260,
    1, 0, MST_BALLISTOMYCETE, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_FUNGUS,
    {TILEP_MONS_BALLISTOMYCETE}, TILE_ERROR
},

{
    MONS_BRIAR_PATCH, 'P', YELLOW, "briar patch",
    M_STATIONARY | M_NOT_DANGEROUS | M_SPINY,
    MR_RES_POISON | MR_VUL_FIRE,
    10, MONS_PLANT, MONS_BRIAR_PATCH, MH_PLANT, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 150,
    10, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 0, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_PLANT,
    {TILEP_MONS_BRIAR_PATCH}, TILE_ERROR
},

// tengu ('Q')
{
    MONS_TENGU, 'Q', LIGHTBLUE, "tengu",
    M_WARM_BLOOD | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_TENGU, MONS_TENGU, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, {AT_PECK, AF_PLAIN, 5}, {AT_CLAW, AF_PLAIN, 5},
       AT_NO_ATK },
    5, 225,
    2, 12, MST_NO_SPELLS, true /*chicken*/, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TENGU}, TILE_CORPSE_TENGU
},

{
    MONS_TENGU_CONJURER, 'Q', BLUE, "tengu conjurer",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    13, MONS_TENGU, MONS_TENGU, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, {AT_PECK, AF_PLAIN, 5}, {AT_CLAW, AF_PLAIN, 5},
       AT_NO_ATK },
    7, 315,
    2, 17, MST_TENGU_CONJURER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TENGU_CONJURER}, TILE_ERROR
},

{
    MONS_TENGU_WARRIOR, 'Q', CYAN, "tengu warrior",
    M_FIGHTER | M_ARCHER | M_WARM_BLOOD | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    13, MONS_TENGU, MONS_TENGU, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 16}, {AT_PECK, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 8},
       AT_NO_ATK },
    10, 600,
    2, 17, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TENGU_WARRIOR}, TILE_ERROR
},

{
    MONS_TENGU_REAVER, 'Q', LIGHTMAGENTA, "tengu reaver",
    M_FIGHTER | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    13, MONS_TENGU, MONS_TENGU, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 27}, {AT_PECK, AF_PLAIN, 11}, {AT_CLAW, AF_PLAIN, 11},
       AT_NO_ATK },
    17, 850,
    2, 17, MST_TENGU_REAVER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TENGU_REAVER}, TILE_ERROR
},

// spiritual beings ('R')

// Dummy species monster
{
    MONS_DJINNI, 'R', MAGENTA, "djinni",
    M_SPEAKS | M_FLIES,
    mrd(MR_RES_FIRE, 2) | MR_VUL_COLD,
    10, MONS_DJINNI, MONS_DJINNI, MH_NONLIVING, 30,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 400,
    5, 5, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EFREET}, TILE_ERROR
},

{
    MONS_EFREET, 'R', RED, "efreet",
    M_SPEAKS | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    12, MONS_EFREET, MONS_EFREET, MH_DEMONIC, 20,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    10, 5, MST_EFREET, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EFREET}, TILE_ERROR
},

{
    MONS_RAKSHASA, 'R', YELLOW, "rakshasa",
    M_SEE_INVIS,
    MR_RES_POISON,
    15, MONS_RAKSHASA, MONS_RAKSHASA, MH_DEMONIC, 140,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 660,
    6, 14, MST_RAKSHASA, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_RAKSHASA}, TILE_ERROR
},

{
    MONS_WENDIGO, 'R', BLUE, "wendigo",
    M_SEE_INVIS,
    MR_RES_POISON | mrd(MR_RES_COLD, 3) | MR_VUL_FIRE,
    10, MONS_WENDIGO, MONS_WENDIGO, MH_DEMONIC, 140,
    { {AT_CLAW, AF_COLD, 30}, {AT_BITE, AF_VAMPIRIC, 20}, AT_NO_ATK, AT_NO_ATK },
    15, 880,
    4, 20, MST_WENDIGO, false, S_HOWL,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_WENDIGO}, TILE_ERROR
},

{
    MONS_DRYAD, 'R', LIGHTGREEN, "dryad",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_VUL_FIRE,
    10, MONS_DRYAD, MONS_DRYAD, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 520,
    6, 12, MST_DRYAD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DRYAD}, TILE_CORPSE_DRYAD
},

{
    MONS_ELEIONOMA, 'R', GREEN, "eleionoma",
    M_WARM_BLOOD | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_ELEIONOMA, MONS_ELEIONOMA, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 800,
    2, 10, MST_ELEIONOMA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ELEIONOMA}, TILE_CORPSE_ELEIONOMA
},

{
    MONS_SNAPLASHER_VINE, 'w', LIGHTGREEN, "snaplasher vine",
    M_NO_EXP_GAIN | M_STATIONARY | M_NO_POLY_TO,
    MR_RES_POISON,
    10, MONS_PLANT, MONS_SNAPLASHER_VINE, MH_PLANT, 40,
    { {AT_CONSTRICT, AF_CRUSH, 0}, {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK },
    12, 240,
    4, 7, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_SNAPLASHER_VINE_SEGMENT, '*', LIGHTGREEN, "snaplasher vine segment",
    M_NOT_DANGEROUS | M_STATIONARY | M_NO_POLY_TO,
    MR_RES_POISON,
    10, MONS_PLANT, MONS_SNAPLASHER_VINE, MH_PLANT, 40,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 360,
    6, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

// snakes ('S')
DUMMY(MONS_SNAKE, 'S', LIGHTGREEN, "snake", TILEP_MONS_ADDER)

{
    MONS_BALL_PYTHON, 'S', GREEN, "ball python",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    13, MONS_SNAKE, MONS_BALL_PYTHON, MH_NATURAL, 0,
    { {AT_BITE, AF_PLAIN, 3}, {AT_CONSTRICT, AF_CRUSH, 1},
       AT_NO_ATK, AT_NO_ATK },
    1, 35,
    0, 11, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 12, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_BALL_PYTHON}, TILE_CORPSE_BALL_PYTHON
},

{
    MONS_ADDER, 'S', LIGHTGREEN, "adder",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_SNAKE, MONS_ADDER, MH_NATURAL, 10,
    { {AT_BITE, AF_POISON, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    1, 15, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 13, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_SNAKE,
    {TILEP_MONS_ADDER}, TILE_CORPSE_ADDER
},

{
    MONS_WATER_MOCCASIN, 'S', BROWN, "water moccasin",
    M_COLD_BLOOD,
    MR_RES_POISON,
    11, MONS_SNAKE, MONS_WATER_MOCCASIN, MH_NATURAL, 20,
    { {AT_BITE, AF_POISON, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    2, 15, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 14, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_SNAKE,
    {TILEP_MONS_WATER_MOCCASIN}, TILE_CORPSE_WATER_MOCCASIN
},

{
    MONS_BLACK_MAMBA, 'S', BLUE, "black mamba",
    M_COLD_BLOOD,
    MR_RES_POISON,
    12, MONS_SNAKE, MONS_BLACK_MAMBA, MH_NATURAL, 20,
    { {AT_BITE, AF_POISON, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    4, 15, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_LAND, 18, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_BLACK_MAMBA}, TILE_CORPSE_BLACK_MAMBA
},

{
    MONS_SEA_SNAKE, 'S', LIGHTRED, "sea snake",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_SNAKE, MONS_SEA_SNAKE, MH_NATURAL, 40,
    { {AT_BITE, AF_POISON_STRONG, 24}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 650,
    2, 15, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 12, SWIM_ENERGY(4),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_SEA_SNAKE}, TILE_CORPSE_SEA_SNAKE
},

{
    MONS_ANACONDA, 'S', LIGHTGREY, "anaconda",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    10, MONS_SNAKE, MONS_ANACONDA, MH_NATURAL, 40,
    { {AT_CONSTRICT, AF_CRUSH, 6}, {AT_BITE, AF_PLAIN, 20},
       AT_NO_ATK, AT_NO_ATK },
    11, 605,
    4, 16, MST_NO_SPELLS, true, S_HISS,
    I_ANIMAL, HT_AMPHIBIOUS, 18, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_ANACONDA}, TILE_CORPSE_ANACONDA
},

{
    MONS_GUARDIAN_SERPENT, 'S', WHITE, "guardian serpent",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS,
    MR_RES_POISON,
    10, MONS_GUARDIAN_SERPENT, MONS_GUARDIAN_SERPENT, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    6, 14, MST_GUARDIAN_SERPENT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_GUARDIAN_SERPENT}, TILE_CORPSE_GUARDIAN_SERPENT
},

{
    MONS_SHOCK_SERPENT, 'S', LIGHTBLUE, "shock serpent",
    M_COLD_BLOOD,
    mrd(MR_RES_ELEC, 3),
    12, MONS_SNAKE, MONS_SHOCK_SERPENT, MH_NATURAL, 40,
    { {AT_BITE, AF_ELEC, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    2, 15, MST_SHOCK_SERPENT, true, S_HISS,
    I_ANIMAL, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_SHOCK_SERPENT}, TILE_CORPSE_SHOCK_SERPENT
},

{
    MONS_MANA_VIPER, 'S', MAGENTA, "mana viper",
    M_COLD_BLOOD | M_SEE_INVIS,
    MR_RES_POISON,
    10, MONS_SNAKE, MONS_MANA_VIPER, MH_NATURAL, 100,
    { {AT_BITE, AF_ANTIMAGIC, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495,
    3, 14, MST_NO_SPELLS, false, S_HISS,
    I_ANIMAL, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_SNAKE,
    {TILEP_MONS_MANA_VIPER}, TILE_ERROR
},

// trolls ('T')
{
    MONS_TROLL, 'T', BROWN, "troll",
    M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN,
    MR_NO_FLAGS,
    10, MONS_TROLL, MONS_TROLL, MH_NATURAL, 20,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 15},
      {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    7, 385,
    3, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TROLL}, TILE_CORPSE_TROLL
},

{
    MONS_IRON_TROLL, 'T', CYAN, "iron troll",
    M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN,
    MR_RES_FIRE | MR_RES_COLD,
    10, MONS_TROLL, MONS_IRON_TROLL, MH_NATURAL, 100,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 25},
      {AT_CLAW, AF_PLAIN, 25}, AT_NO_ATK },
    16, 880,
    20, 4, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRON_TROLL}, TILE_CORPSE_IRON_TROLL
},

{
    MONS_DEEP_TROLL, 'T', YELLOW, "deep troll",
    M_WARM_BLOOD | M_SEE_INVIS | M_UNBLINDABLE | M_SPEAKS | M_FAST_REGEN,
    MR_NO_FLAGS,
    9, MONS_TROLL, MONS_DEEP_TROLL, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 27}, {AT_CLAW, AF_PLAIN, 20},
      {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    10, 550,
    6, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_TROLL}, TILE_CORPSE_DEEP_TROLL
},

{
    MONS_DEEP_TROLL_EARTH_MAGE, 'T', MAGENTA, "deep troll earth mage",
    M_WARM_BLOOD | M_SEE_INVIS | M_UNBLINDABLE | M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_TROLL, MONS_DEEP_TROLL, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 27}, {AT_CLAW, AF_PLAIN, 20},
      {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    12, 480,
    // the extra AC is essentially a perma-stoneskin
    12, 10, MST_DEEP_TROLL_EARTH_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_TROLL_EARTH_MAGE}, TILE_ERROR
},

{
    MONS_DEEP_TROLL_SHAMAN, 'T', WHITE, "deep troll shaman",
    M_WARM_BLOOD | M_SEE_INVIS | M_UNBLINDABLE | M_SPEAKS,
    MR_NO_FLAGS,
    12, MONS_TROLL, MONS_DEEP_TROLL, MH_NATURAL, 40,
    { {AT_BITE, AF_PLAIN, 27}, {AT_CLAW, AF_PLAIN, 20},
      {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    12, 480,
    6, 10, MST_DEEP_TROLL_SHAMAN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEEP_TROLL_SHAMAN}, TILE_ERROR
},

// vampires ('V')
{
    MONS_VAMPIRE, 'V', RED, "vampire",
    M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD,
    MR_RES_COLD,
    11, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, 40,
    { {AT_HIT, AF_PLAIN, 15}, {AT_BITE, AF_VAMPIRIC, 15}, AT_NO_ATK,
       AT_NO_ATK },
    6, 330,
    10, 10, MST_VAMPIRE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAMPIRE}, TILE_ERROR
},

{
    MONS_VAMPIRE_KNIGHT, 'V', CYAN, "vampire knight",
    M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD,
    MR_RES_COLD,
    16, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, 80,
    { {AT_HIT, AF_PLAIN, 33}, {AT_BITE, AF_VAMPIRIC, 15}, AT_NO_ATK,
       AT_NO_ATK },
    11, 715,
    10, 10, MST_VAMPIRE_KNIGHT, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAMPIRE_KNIGHT}, TILE_ERROR
},

{
    MONS_VAMPIRE_MAGE, 'V', MAGENTA, "vampire mage",
    M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD | M_FLIES,
    MR_RES_COLD,
    15, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, 80,
    { {AT_HIT, AF_PLAIN, 15}, {AT_BITE, AF_VAMPIRIC, 15}, AT_NO_ATK,
       AT_NO_ATK },
    10, 550,
    10, 10, MST_VAMPIRE_MAGE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VAMPIRE_MAGE}, TILE_ERROR
},

{
    MONS_JIANGSHI, 'V', YELLOW, "jiangshi",
    M_FIGHTER,
    MR_RES_COLD,
    10, MONS_JIANGSHI, MONS_VAMPIRE, MH_UNDEAD, 80,
    { {AT_CLAW, AF_VAMPIRIC, 27}, {AT_CLAW, AF_VAMPIRIC, 27}, AT_NO_ATK,
       AT_NO_ATK },
    10, 650,
    10, 10, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 18, MOVE_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JIANGSHI}, TILE_ERROR
},

// incorporeal undead ('W')
{ // dummy genus monster, but also used for removed monsters
    MONS_GHOST, 'W', WHITE, "ghost",
    M_INSUBSTANTIAL | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    0, MONS_GHOST, MONS_GHOST, MH_UNDEAD, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GHOST}, TILE_ERROR
},

{
    MONS_PHANTOM, 'W', BLUE, "phantom",
    M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_COLD, 2),
    7, MONS_PHANTOM, MONS_PHANTOM, MH_UNDEAD, 40,
    { {AT_HIT, AF_BLINK_WITH, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 270,
    3, 13, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PHANTOM}, TILE_ERROR
},

{
    MONS_FLAYED_GHOST, 'W', RED, "flayed ghost",
    M_INSUBSTANTIAL | M_SPEAKS | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_PHANTOM, MONS_FLAYED_GHOST, MH_UNDEAD, 60,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 605,
    0, 14, MST_FLAYED_GHOST, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FLAYED_GHOST}, TILE_ERROR
},

// player ghost - stats are stored in ghost struct
{
    MONS_PLAYER_GHOST, 'W', WHITE, "player ghost",
    M_FIGHTER | M_SPEAKS | M_INSUBSTANTIAL | M_NO_POLY_TO | M_FLIES | M_GHOST_DEMON,
    MR_NO_FLAGS,
    15, MONS_PHANTOM, MONS_PLAYER_GHOST, MH_UNDEAD, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 140,
    1, 2, MST_GHOST, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PLAYER_GHOST}, TILE_ERROR
},


{
    MONS_SHADOW, 'W', MAGENTA, "shadow",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_COLD, 3),
    18, MONS_WRAITH, MONS_SHADOW, MH_UNDEAD, 40,
    { {AT_HIT, AF_SHADOWSTAB, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 240,
    7, 10, MST_SHADOW, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SHADOW}, TILE_ERROR
},

{
    MONS_SILENT_SPECTRE, 'W', CYAN, "silent spectre",
    M_SPEAKS /* uh... */ | M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_COLD, 3),
    10, MONS_WRAITH, MONS_SILENT_SPECTRE, MH_UNDEAD, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    5, 15, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SILENT_SPECTRE}, TILE_ERROR
},

{
    MONS_DROWNED_SOUL, 'W', LIGHTCYAN, "drowned soul",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_SPEAKS | M_FLIES,
    mrd(MR_RES_COLD, 3),
    8, MONS_PHANTOM, MONS_DROWNED_SOUL, MH_UNDEAD, 60,
    { {AT_TOUCH, AF_DROWN, 0}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 195,
    0, 13, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DROWNED_SOUL}, TILE_ERROR
},

{
    MONS_WRAITH, 'W', LIGHTGREY, "wraith",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_COLD,
    14, MONS_WRAITH, MONS_WRAITH, MH_UNDEAD, 60,
    { {AT_HIT, AF_DRAIN_SPEED, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    10, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_WRAITH}, TILE_ERROR
},

{
    MONS_SHADOW_WRAITH, 'W', LIGHTMAGENTA, "shadow wraith",
    M_SEE_INVIS | M_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_NO_FLAGS,
    15, MONS_WRAITH, MONS_SHADOW_WRAITH, MH_UNDEAD, 100,
    { {AT_HIT, AF_DRAIN_SPEED, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    7, 7, MST_NO_SPELLS, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SHADOW_WRAITH}, TILE_ERROR
},

{
    MONS_PUTRID_MOUTH, 'W', GREEN, "putrid mouth",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_NO_FLAGS,
    15, MONS_WRAITH, MONS_PUTRID_MOUTH, MH_UNDEAD, 100,
    { {AT_BITE, AF_POISON_STRONG, 30}, {AT_CLAW, AF_DRAIN_STAT, 20}, AT_NO_ATK, AT_NO_ATK },
    14, 700,
    5, 14, MST_PUTRID_MOUTH, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PUTRID_MOUTH}, TILE_ERROR
},

{
    MONS_FREEZING_WRAITH, 'W', LIGHTBLUE, "freezing wraith",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    10, MONS_WRAITH, MONS_FREEZING_WRAITH, MH_UNDEAD, 40,
    { {AT_HIT, AF_COLD, 16}, {AT_HIT, AF_DRAIN_SPEED, 15}, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    12, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FREEZING_WRAITH}, TILE_ERROR
},

{
    MONS_EIDOLON, 'W', BROWN, "eidolon",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_COLD,
    14, MONS_WRAITH, MONS_EIDOLON, MH_UNDEAD, 140,
    { {AT_HIT, AF_DRAIN_SPEED, 27}, {AT_HIT, AF_DRAIN_STAT, 17}, AT_NO_ATK,
       AT_NO_ATK },
    13, 715,
    12, 10, MST_EIDOLON, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EIDOLON}, TILE_ERROR
},

{
    MONS_PHANTASMAL_WARRIOR, 'W', LIGHTGREEN, "phantasmal warrior",
    M_FIGHTER | M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_COLD,
    13, MONS_WRAITH, MONS_PHANTASMAL_WARRIOR, MH_UNDEAD, 80,
    { {AT_HIT, AF_VULN, 39}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 495,
    12, 10, MST_PHANTASMAL_WARRIOR, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PHANTASMAL_WARRIOR}, TILE_ERROR
},

// large abominations ('X')
{
    MONS_ABOMINATION_LARGE, 'X', LIGHTRED, "large abomination",
    M_NO_REGEN,
    MR_NO_FLAGS,
    10, MONS_ABOMINATION_SMALL, MONS_ABOMINATION_LARGE, MH_UNDEAD, 100,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 495,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_ABOMINATION_LARGE, TVARY_MOD}, TILE_ERROR
},

{
    MONS_THRASHING_HORROR, 'X', YELLOW, "thrashing horror",
    M_BATTY | M_NO_POLY_TO,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_THRASHING_HORROR, MONS_THRASHING_HORROR, MH_NONLIVING, 60,
    { {AT_TRAMPLE, AF_TRAMPLE, 17}, {AT_TRAMPLE, AF_TRAMPLE, 9},
       AT_NO_ATK, AT_NO_ATK },
    9, 495,
    5, 10, MST_THRASHING_HORROR, false, S_ROAR,
    I_ANIMAL, HT_LAND, 25, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_THRASHING_HORROR}, TILE_ERROR
},

{
    MONS_TENTACLED_MONSTROSITY, 'X', GREEN, "tentacled monstrosity",
    M_SEE_INVIS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_TENTACLED_MONSTROSITY, MONS_TENTACLED_MONSTROSITY,
        MH_DEMONIC, 160,
    { {AT_TENTACLE_SLAP, AF_PLAIN, 22}, {AT_TENTACLE_SLAP, AF_PLAIN, 17},
      {AT_TENTACLE_SLAP, AF_PLAIN, 13}, {AT_CONSTRICT, AF_CRUSH, 9} },
    23, 1265,
    5, 5, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_TENTACLED_MONSTROSITY}, TILE_ERROR
},

{
    MONS_ORB_GUARDIAN, 'X', MAGENTA, "Orb Guardian",
    M_FIGHTER | M_SEE_INVIS | M_NO_POLY_TO,
    MR_NO_FLAGS,
    20, MONS_ORB_GUARDIAN, MONS_ORB_GUARDIAN, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 825,
    13, 13, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_ORB_GUARDIAN}, TILE_ERROR
},

{
    MONS_TENTACLED_STARSPAWN, 'X', LIGHTCYAN, "tentacled starspawn",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    20, MONS_TENTACLED_STARSPAWN, MONS_TENTACLED_STARSPAWN, MH_NONLIVING, 120,
    { {AT_BITE, AF_PLAIN, 40}, {AT_TENTACLE_SLAP, AF_PLAIN, 25}, AT_NO_ATK,
       AT_NO_ATK },
    16, 880,
    5, 5, MST_TENTACLED_STARSPAWN, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_TENTACLED_STARSPAWN}, TILE_ERROR
},

{
    MONS_STARSPAWN_TENTACLE, 'w', LIGHTCYAN, "starspawn tentacle",
    M_NO_EXP_GAIN | M_STATIONARY | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_TENTACLED_STARSPAWN, MONS_STARSPAWN_TENTACLE,
        MH_NONLIVING, WILL_INVULN,
    { {AT_CONSTRICT, AF_CRUSH, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 220,
    8, 2, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_STARSPAWN_TENTACLE_SEGMENT, '*', LIGHTCYAN, "starspawn tentacle segment",
    M_NOT_DANGEROUS | M_STATIONARY | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_TENTACLED_STARSPAWN, MONS_STARSPAWN_TENTACLE_SEGMENT,
        MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 220,
    8, 2, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_STARCURSED_MASS, 'X', BLUE, "starcursed mass",
    M_SEE_INVIS | M_FAST_REGEN,
    MR_NO_FLAGS,
    12, MONS_STARCURSED_MASS, MONS_STARCURSED_MASS, MH_NONLIVING, 100,
    { {AT_TOUCH, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 1500,
    10, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_STARCURSED_MASS}, TILE_ERROR
},

// Summoning miscast, monster is scaled based on miscast strength
{
    MONS_NAMELESS, 'X', CYAN, "nameless horror",
    M_SEE_INVIS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    11, MONS_NAMELESS, MONS_NAMELESS, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_ANTIMAGIC, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 600,
    8, 2, MST_NAMELESS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_NAMELESS_HORROR}, TILE_ERROR
},

// yaks, sheep, elephants, and other beasts ('Y')
{
    MONS_DREAM_SHEEP, 'Y', CYAN, "dream sheep",
    M_WARM_BLOOD | M_HERD,
    MR_NO_FLAGS,
    10, MONS_DREAM_SHEEP, MONS_DREAM_SHEEP, MH_NATURAL, 30,
    { {AT_BITE, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 220,
    2, 10, MST_DREAM_SHEEP, true, S_BLEAT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_DREAM_SHEEP}, TILE_CORPSE_DREAM_SHEEP,
},

{
    MONS_YAK, 'Y', BROWN, "yak",
    M_WARM_BLOOD | M_HERD,
    MR_NO_FLAGS,
    9, MONS_YAK, MONS_YAK, MH_NATURAL, 20,
    { {AT_GORE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    4, 7, MST_NO_SPELLS, true, S_BELLOW,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_YAK}, TILE_CORPSE_YAK
},

{
    MONS_DEATH_YAK, 'Y', YELLOW, "death yak",
    M_WARM_BLOOD | M_HERD,
    MR_NO_FLAGS,
    8, MONS_YAK, MONS_DEATH_YAK, MH_NATURAL, 100,
    { {AT_GORE, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    9, 5, MST_NO_SPELLS, true, S_BELLOW,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_DEATH_YAK}, TILE_CORPSE_DEATH_YAK
},

{
    MONS_CATOBLEPAS, 'Y', MAGENTA, "catoblepas",
    M_WARM_BLOOD,
    MR_RES_PETRIFY,
    8, MONS_YAK, MONS_CATOBLEPAS, MH_NATURAL, 100,
    { {AT_GORE, AF_PLAIN, 36}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    10, 2, MST_CATOBLEPAS, true, S_BELLOW,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_CATOBLEPAS}, TILE_CORPSE_CATOBLEPAS
},

{
    MONS_ELEPHANT, 'Y', GREEN, "elephant",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    9, MONS_ELEPHANT, MONS_ELEPHANT, MH_NATURAL, 60,
    { {AT_TRAMPLE, AF_TRAMPLE, 20}, {AT_TRUNK_SLAP, AF_PLAIN, 5}, AT_NO_ATK,
       AT_NO_ATK },
    9, 675,
    8, 2, MST_NO_SPELLS, true, S_TRUMPET,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_ELEPHANT}, TILE_CORPSE_ELEPHANT
},

{
    MONS_DIRE_ELEPHANT, 'Y', BLUE, "dire elephant",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    8, MONS_ELEPHANT, MONS_DIRE_ELEPHANT, MH_NATURAL, 100,
    { {AT_TRAMPLE, AF_TRAMPLE, 40}, {AT_TRUNK_SLAP, AF_PLAIN, 15}, AT_NO_ATK,
       AT_NO_ATK },
    15, 1125,
    13, 2, MST_NO_SPELLS, true, S_TRUMPET,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_DIRE_ELEPHANT}, TILE_CORPSE_DIRE_ELEPHANT
},

{
    MONS_HELLEPHANT, 'Y', LIGHTRED, "hellephant",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    10, MONS_ELEPHANT, MONS_HELLEPHANT, MH_DEMONIC, 140,
    { {AT_TRAMPLE, AF_TRAMPLE, 45}, {AT_BITE, AF_PLAIN, 20},
      {AT_GORE, AF_PLAIN, 15 }, AT_NO_ATK },
    20, 1700,
    13, 10, MST_HELLEPHANT, true, S_TRUMPET,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HELLEPHANT}, TILE_CORPSE_HELLEPHANT
},

{
    MONS_APIS, 'Y', WHITE, "apis",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    8, MONS_APIS, MONS_APIS, MH_HOLY, 100,
    { {AT_GORE, AF_HOLY, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1200,
    9, 5, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_APIS}, TILE_CORPSE_APIS
},

{
    MONS_ICE_BEAST, 'Y', LIGHTBLUE, "ice beast",
    M_NO_FLAGS,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    13, MONS_ICE_BEAST, MONS_ICE_BEAST, MH_NATURAL, 20,
    { {AT_HIT, AF_COLD, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    5, 10, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_ICE_BEAST}, TILE_ERROR
},

{
    MONS_SKY_BEAST, 'Y', LIGHTCYAN, "sky beast",
    M_NO_SKELETON | M_FLIES,
    mrd(MR_RES_ELEC, 3),
    13, MONS_SKY_BEAST, MONS_SKY_BEAST, MH_NATURAL, 20,
    { {AT_HIT, AF_ELEC, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    3, 13, MST_NO_SPELLS, true, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SKY_BEAST}, TILE_CORPSE_SKY_BEAST
},

// zombies and derived undead ('Z')
// zombie, skeleton and simulacra species depend on corpse species,
// or else are chosen randomly
{
    MONS_ZOMBIE, 'Z', BROWN, "zombie",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_ZOMBIE, MONS_ZOMBIE, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 75,
    0, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_ZOMBIE_SMALL}, TILE_ERROR
},

{
    MONS_SKELETON, 'Z', LIGHTGREY, "skeleton",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_SKELETON, MONS_SKELETON, MH_UNDEAD, -1,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    0, 0,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_SKELETON_SMALL}, TILE_ERROR
},

{
    MONS_SIMULACRUM, 'Z', LIGHTBLUE, "simulacrum",
    M_NO_REGEN,
    MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    9, MONS_SIMULACRUM, MONS_SIMULACRUM, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    10, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_SIMULACRUM_SMALL}, TILE_ERROR
},

{
    MONS_ZOMBIE_SMALL, 'Z', BROWN, "small zombie",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_ZOMBIE, MONS_ZOMBIE, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 75,
    0, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_ZOMBIE_SMALL}, TILE_ERROR
},

{
    MONS_SKELETON_SMALL, 'Z', LIGHTGREY, "small skeleton",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_SKELETON, MONS_SKELETON, MH_UNDEAD, -1,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    0, 0,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_SKELETON_SMALL}, TILE_ERROR
},

{
    MONS_SIMULACRUM_SMALL, 'Z', LIGHTBLUE, "small simulacrum",
    M_NO_REGEN,
    MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    9, MONS_SIMULACRUM, MONS_SIMULACRUM, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    10, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_SIMULACRUM_SMALL}, TILE_ERROR
},

{
    MONS_ZOMBIE_LARGE, 'Z', YELLOW, "large zombie",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_ZOMBIE, MONS_ZOMBIE, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    8, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_ZOMBIE_LARGE}, TILE_ERROR
},

{
    MONS_SKELETON_LARGE, 'Z', WHITE, "large skeleton",
    M_NO_REGEN,
    mrd(MR_RES_COLD, 2),
    9, MONS_SKELETON, MONS_SKELETON, MH_UNDEAD, -1,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    0, 0,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 5, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_SKELETON_LARGE}, TILE_ERROR
},

{
    MONS_SIMULACRUM_LARGE, 'Z', LIGHTCYAN, "large simulacrum",
    M_NO_REGEN,
    MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    9, MONS_SIMULACRUM, MONS_SIMULACRUM, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    10, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_SIMULACRUM_LARGE}, TILE_ERROR
},

// spectral thing - similar to zombies/skeletons
{
    MONS_SPECTRAL_THING, 'Z', GREEN, "spectral thing",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_COLD,
    11, MONS_WRAITH, MONS_SPECTRAL_THING, MH_UNDEAD, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    8, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_SPECTRAL_LARGE}, TILE_ERROR
},

// yred bound soul - similar to a spectral thing, stats overridden
// on creation
{
    MONS_BOUND_SOUL, 'Z', LIGHTRED, "bound soul",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_COLD,
    11, MONS_WRAITH, MONS_SPECTRAL_THING, MH_UNDEAD, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    8, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_SPECTRAL_LARGE}, TILE_ERROR
},


// water monsters
{
    MONS_ELECTRIC_EEL, 'S', LIGHTCYAN, "electric eel",
    M_NO_GEN_DERIVED | M_COLD_BLOOD,
    mrd(MR_RES_ELEC, 3),
    19, MONS_ELECTRIC_EEL, MONS_ELECTRIC_EEL, MH_NATURAL, 10,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    1, 15, MST_ZAPPER, true, S_SILENT,
    I_ANIMAL, HT_WATER, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_SNAKE,
    {TILEP_MONS_ELECTRIC_EEL}, TILE_CORPSE_ELECTRIC_EEL
},

// A kraken and its tentacles get a random colour from ETC_KRAKEN.
{
    MONS_KRAKEN, 'X', LIGHTMAGENTA, "kraken",
    M_NO_SKELETON | M_COLD_BLOOD,
    MR_NO_FLAGS,
    6, MONS_KRAKEN, MONS_KRAKEN, MH_NATURAL, 60,
    { {AT_BITE, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 2080,
    20, 0, MST_KRAKEN, true, S_SILENT,
    I_ANIMAL, HT_WATER, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_KRAKEN_HEAD}, TILE_CORPSE_KRAKEN
},

{
    MONS_KRAKEN_TENTACLE, 'w', LIGHTMAGENTA, "tentacle",
    M_COLD_BLOOD | M_NO_EXP_GAIN | M_STATIONARY | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_KRAKEN, MONS_KRAKEN_TENTACLE, MH_NATURAL, WILL_INVULN,
    { {AT_TENTACLE_SLAP, AF_PLAIN, 29}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 480,
    5, 7, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 17, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_KRAKEN_TENTACLE_SEGMENT, '*', LIGHTMAGENTA, "tentacle segment",
    M_COLD_BLOOD | M_NOT_DANGEROUS | M_STATIONARY | M_SUBMERGES | M_NO_POLY_TO | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_KRAKEN, MONS_KRAKEN_TENTACLE_SEGMENT, MH_NATURAL, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    12, 480,
    5, 7, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 18, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

// Octopode race, not a 'normal' octopus.
// Dummy monster for recolouring.
{
    MONS_OCTOPODE, 'x', LIGHTCYAN, "octopode",
    M_NO_SKELETON | M_SPEAKS | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_NO_FLAGS,
    10, MONS_OCTOPODE, MONS_OCTOPODE, MH_NATURAL, 10,
    { {AT_TENTACLE_SLAP, AF_PLAIN, 24}, {AT_CONSTRICT, AF_CRUSH, 5},
       AT_NO_ATK, AT_NO_ATK },
    8, 560,
    1, 5, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_OCTOPODE}, TILE_CORPSE_OCTOPODE
},

// lava monsters
{
    MONS_LAVA_SNAKE, 'S', YELLOW, "lava snake",
    M_WARM_BLOOD,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    17, MONS_SNAKE, MONS_LAVA_SNAKE, MH_NATURAL, 10,
    { {AT_BITE, AF_FIRE, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    2, 17, MST_LAVA_SNAKE, true, S_HISS,
    I_ANIMAL, HT_LAVA, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_SNAKE,
    {TILEP_MONS_LAVA_SNAKE}, TILE_CORPSE_LAVA_SNAKE
},

{
    MONS_SALAMANDER, 'N', LIGHTRED, "salamander",
    M_FIGHTER | M_WARM_BLOOD,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    10, MONS_NAGA, MONS_SALAMANDER, MH_NATURAL, 40,
    { {AT_HIT, AF_FIRE, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 640,
    5, 7, MST_NO_SPELLS, true, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS_LAVA, 10, SWIM_ENERGY(7),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_NAGA,
    {TILEP_MONS_SALAMANDER}, TILE_CORPSE_SALAMANDER
},

{
    MONS_SALAMANDER_MYSTIC, 'N', YELLOW, "salamander mystic",
    M_WARM_BLOOD,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    10, MONS_NAGA, MONS_SALAMANDER, MH_NATURAL, 60,
    { {AT_HIT, AF_FIRE, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 650,
    5, 7, MST_SALAMANDER_MYSTIC, true, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS_LAVA, 10, SWIM_ENERGY(7),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_NAGA,
    {TILEP_MONS_SALAMANDER_MYSTIC}, TILE_ERROR
},

{
    MONS_SALAMANDER_TYRANT, 'N', RED, "salamander tyrant",
    M_WARM_BLOOD,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    10, MONS_NAGA, MONS_SALAMANDER, MH_NATURAL, 60,
    { {AT_HIT, AF_FIRE, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 675,
    5, 7, MST_SALAMANDER_TYRANT, true, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS_LAVA, 10, SWIM_ENERGY(7),
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_NAGA,
    {TILEP_MONS_SALAMANDER_TYRANT}, TILE_ERROR
},

// Semi-unique humans ('@')
{
    MONS_HELLBINDER, '@', ETC_FIRE, "Hellbinder",
    M_SPEAKS | M_WARM_BLOOD | M_SEE_INVIS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1500,
    0, 13, MST_HELLBINDER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HELLBINDER}, TILE_ERROR
},

{
    MONS_CLOUD_MAGE, '@', ETC_SILVER, "Cloud Mage",
    M_SPEAKS | M_WARM_BLOOD | M_SEE_INVIS | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1500,
    0, 13, MST_CLOUD_MAGE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CLOUD_MAGE}, TILE_ERROR
},

{
    MONS_HEADMASTER, '@', ETC_IRON, "Head Instructor",
    M_WARM_BLOOD | M_FIGHTER | M_TWO_WEAPONS | M_SPEAKS | M_SEE_INVIS,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 30}, {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    20, 1500,
    0, 18, MST_HEADMASTER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HEADMASTER}, TILE_ERROR
},

// 'dummy' a fairly accurate description
DUMMY(MONS_PLAYER, '@', LIGHTGREY, "player", TILEP_MONS_PLAYER_GHOST)

// player illusion (Mara) - stats are stored in ghost struct. Undead/demonic
// flags are set based on the current player's species!
{
    MONS_PLAYER_ILLUSION, '@', WHITE, "player illusion",
    M_FIGHTER | M_SPEAKS | M_INSUBSTANTIAL | M_NO_POLY_TO | M_FLIES | M_GHOST_DEMON,
    MR_RES_POISON,
    15, MONS_PLAYER_ILLUSION, MONS_PLAYER_ILLUSION, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 140,
    1, 2, MST_GHOST, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PLAYER_GHOST}, TILE_ERROR
},

{
    MONS_PLAYER_SHADOW, 'W', MAGENTA, "shadow",
    M_CANT_SPAWN | M_NO_EXP_GAIN | M_INSUBSTANTIAL | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_HUMAN, MONS_HUMAN, MH_NONLIVING, -3,
    { {AT_HIT, AF_PLAIN, 0}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    3, 13, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},
// dancing weapon
// These are named more explicitly when they attack, also when you use 'x'
// to examine them.
{
    MONS_DANCING_WEAPON, '(', COLOUR_UNDEF, "dancing weapon",
    M_FIGHTER | M_FLIES | M_PREFER_RANGED | M_GHOST_DEMON,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | mrd(MR_RES_ELEC, 3),
    10, MONS_DANCING_WEAPON, MONS_DANCING_WEAPON, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 150,
    10, 20, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILE_UNSEEN_WEAPON}
},

// spectral weapon, for skalds!
{
    MONS_SPECTRAL_WEAPON, '(', GREEN, "spectral weapon",
    M_INSUBSTANTIAL | M_NO_REGEN | M_AVATAR | M_NO_EXP_GAIN | M_FLIES
        | M_GHOST_DEMON,
    MR_RES_FIRE | MR_RES_COLD |  MR_RES_ELEC,
    11, MONS_WRAITH, MONS_SPECTRAL_WEAPON, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 250,
    5, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_SPECTRAL_SBL}, TILE_ERROR
},

// animated armour - variable traits
{
    MONS_ANIMATED_ARMOUR, '[', GREEN, "animated armour",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    10, MONS_ANIMATED_ARMOUR, MONS_ANIMATED_ARMOUR, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 1}, {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK },
    15, 400,
    8, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, MOVE_ENERGY(30),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANIMATED_ARMOUR}, TILE_ERROR
},

// living spells and things that make em
{
    MONS_LIVING_SPELL, '*', LIGHTBLUE, "living spell",
    M_INSUBSTANTIAL | M_NO_EXP_GAIN | M_FLIES,
    MR_NO_FLAGS,
    10, MONS_LIVING_SPELL, MONS_LIVING_SPELL, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 3,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_ORB,
    {TILEP_MONS_LIVING_SPELL}, TILE_ERROR // XXX TODO
},

DUMMY(MONS_WALKING_TOME, ';', BROWN, "walking tome", TILE_ERROR) // XXX TODO

{
    MONS_EARTHEN_TOME, ';', MAGENTA, "walking earthen tome",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    7, MONS_WALKING_TOME, MONS_EARTHEN_TOME, MH_NONLIVING, WILL_INVULN,
    { { AT_HIT, AF_PLAIN, 50 }, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    27, 1000,
    20, 5, MST_WALKING_TOME, false, S_RUSTLE,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_EARTHEN_TOME}, TILE_ERROR // XXX TODO
},

{
    MONS_CRYSTAL_TOME, ';', CYAN, "walking crystal tome",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    14, MONS_WALKING_TOME, MONS_EARTHEN_TOME, MH_NONLIVING, WILL_INVULN,
    { { AT_HIT, AF_PLAIN, 40 }, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1000,
    15, 5, MST_WALKING_TOME, false, S_RUSTLE,
    I_BRAINLESS, HT_LAND, 10, ATTACK_ENERGY(20),
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_CRYSTAL_TOME}, TILE_ERROR // XXX TODO
},

{
    MONS_DIVINE_TOME, ';', LIGHTGREEN /*high priest*/, "walking divine tome",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    14, MONS_WALKING_TOME, MONS_EARTHEN_TOME, MH_NONLIVING, WILL_INVULN,
    { { AT_HIT, AF_PLAIN, 20 }, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1000,
    10, 5, MST_WALKING_TOME, false, S_RUSTLE,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_DIVINE_TOME}, TILE_ERROR // XXX TODO
},

{
    MONS_FROSTBOUND_TOME, ';', YELLOW, "walking frostbound tome",
    M_NO_FLAGS,
    MR_RES_COLD | MR_VUL_FIRE,
    14, MONS_WALKING_TOME, MONS_EARTHEN_TOME, MH_NONLIVING, WILL_INVULN,
    { { AT_HIT, AF_COLD, 20 }, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1000,
    10, 10, MST_WALKING_TOME, false, S_RUSTLE,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_FROSTBOUND_TOME}, TILE_ERROR // XXX TODO
},

// Demonic tentacle things.
{
    MONS_ELDRITCH_TENTACLE, 'w', COLOUR_UNDEF, "eldritch tentacle",
    M_NO_POLY_TO | M_STATIONARY | M_SEE_INVIS | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC
        | MR_RES_ACID, 3) | MR_RES_STICKY_FLAME,
    10, MONS_ELDRITCH_TENTACLE, MONS_ELDRITCH_TENTACLE,
        MH_NONLIVING, WILL_INVULN,
    { {AT_TENTACLE_SLAP, AF_CHAOTIC, 30}, {AT_CLAW, AF_CHAOTIC, 40}, AT_NO_ATK,
       AT_NO_ATK },
    16, 1200,
    13, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_SNAKE,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_ELDRITCH_TENTACLE_SEGMENT, '*', COLOUR_UNDEF, "eldritch tentacle segment",
    M_NOT_DANGEROUS | M_STATIONARY | M_NO_POLY_TO | M_SEE_INVIS | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC
        | MR_RES_ACID, 3) | MR_RES_STICKY_FLAME,
    10, MONS_ELDRITCH_TENTACLE, MONS_ELDRITCH_TENTACLE_SEGMENT,
        MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1200,
    13, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_AMPHIBIOUS, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

// demonspawn ('6')
{
    MONS_DEMONSPAWN, '6', BROWN, "demonspawn",
    M_WARM_BLOOD | M_SPEAKS | M_NO_POLY_TO,
    MR_NO_FLAGS,
    10, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 40,
    { {AT_HIT, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 20},
      {AT_GORE, AF_PLAIN, 20}, AT_NO_ATK },
    10, 925,
    3, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMONSPAWN}, TILE_CORPSE_DEMONSPAWN
},

{
    MONS_DEMONSPAWN_BLOOD_SAINT, '6', LIGHTBLUE, "demonspawn blood saint",
    M_WARM_BLOOD | M_SPEAKS,
    mrd(MR_RES_FIRE, 3),
    10, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 60,
    { {AT_HIT, AF_FIRE, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 925,
    6, 12, MST_DEMONSPAWN_BLOOD_SAINT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMONSPAWN_BLOOD_SAINT}, TILE_ERROR
},

{
    MONS_DEMONSPAWN_WARMONGER, '6', LIGHTCYAN, "demonspawn warmonger",
    M_WARM_BLOOD | M_SPEAKS | M_FIGHTER,
    MR_NO_FLAGS,
    10, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 40}, {AT_CLAW, AF_PLAIN, 20},
      {AT_GORE, AF_PLAIN, 20}, AT_NO_ATK },
    15, 1250,
    3, 12, MST_DEMONSPAWN_WARMONGER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMONSPAWN_WARMONGER}, TILE_ERROR
},

{
    MONS_DEMONSPAWN_CORRUPTER, '6', LIGHTGREEN, "demonspawn corrupter",
    M_WARM_BLOOD | M_SPEAKS,
    mrd(MR_RES_NEG, 3),
    10, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 25}, {AT_KICK, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    15, 925,
    3, 15, MST_DEMONSPAWN_CORRUPTER, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMONSPAWN_CORRUPTER}, TILE_ERROR
},

{
    MONS_DEMONSPAWN_BLACK_SUN, '6', LIGHTMAGENTA, "demonspawn black sun",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS,
    mrd(MR_RES_COLD, 3),
    10, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 925,
    9, 12, MST_DEMONSPAWN_BLACK_SUN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEMONSPAWN_BLACK_SUN}, TILE_ERROR
},

// minor demons: imps, etc. ('5')
{
    MONS_CRIMSON_IMP, '5', RED, "crimson imp",
    M_SPEAKS | M_FAST_REGEN | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    13, MONS_CRIMSON_IMP, MONS_CRIMSON_IMP, MH_DEMONIC, 40,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 135,
    3, 14, MST_BLINKER, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE,
         MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_CRIMSON_IMP}, TILE_ERROR
},

{
    MONS_QUASIT, '5', LIGHTGREY, "quasit",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    13, MONS_QUASIT, MONS_QUASIT, MH_DEMONIC, 10,
    { {AT_BITE, AF_DRAIN_DEX, 3}, {AT_CLAW, AF_DRAIN_DEX, 2},
      {AT_CLAW, AF_DRAIN_DEX, 2}, AT_NO_ATK },
    3, 150,
    5, 17, MST_NO_SPELLS, false, S_MOAN,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_QUASIT}, TILE_ERROR
},

{
    MONS_WHITE_IMP, '5', WHITE, "white imp",
    M_SPEAKS | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | MR_VUL_FIRE,
    10, MONS_WHITE_IMP, MONS_WHITE_IMP, MH_DEMONIC, 10,
    { {AT_HIT, AF_COLD, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    4, 10, MST_WHITE_IMP, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LITTLE,
         MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_WHITE_IMP}, TILE_ERROR
},

{
    MONS_UFETUBUS, '5', LIGHTCYAN, "ufetubus",
    M_NO_FLAGS,
    MR_VUL_FIRE | MR_RES_COLD,
    28, MONS_UFETUBUS, MONS_UFETUBUS, MH_DEMONIC, 10,
    { {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    1, 70,
    2, 15, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_UFETUBUS}, TILE_ERROR
},

{
    MONS_IRON_IMP, '5', CYAN, "iron imp",
    M_SPEAKS,
    MR_RES_POISON | MR_RES_ELEC | mrd(MR_RES_FIRE, 3) | MR_RES_COLD,
    14, MONS_IRON_IMP, MONS_IRON_IMP, MH_DEMONIC, 10,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 165,
    6, 8, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRON_IMP}, TILE_ERROR
},

{
    MONS_SHADOW_IMP, '5', MAGENTA, "shadow imp",
    M_SPEAKS,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    11, MONS_SHADOW_IMP, MONS_SHADOW_IMP, MH_DEMONIC, 10,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 110,
    3, 11, MST_SHADOW_IMP, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SHADOW_IMP}, TILE_ERROR
},

{
    MONS_LEMURE, '5', YELLOW, "lemure",
    M_SPEAKS,
    MR_NO_FLAGS,
    10, MONS_LEMURE, MONS_LEMURE, MH_DEMONIC, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 275,
    4, 12, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LEMURE}, TILE_ERROR
},

// devils etc. ('4')
{
    MONS_RUST_DEVIL, '4', BROWN, "rust devil",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_ELEC | mrd(MR_RES_FIRE, 3) | MR_RES_COLD,
    15, MONS_RUST_DEVIL, MONS_RUST_DEVIL, MH_DEMONIC, 60,
    { {AT_HIT, AF_CORRODE, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    10, 8, MST_NO_SPELLS, false, S_SCREECH,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_RUST_DEVIL}, TILE_ERROR
},

{
    MONS_ORANGE_DEMON, '4', LIGHTRED, "orange demon",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    12, MONS_ORANGE_DEMON, MONS_ORANGE_DEMON, MH_DEMONIC, 60,
    { {AT_STING, AF_REACH_STING, 15}, {AT_HIT, AF_WEAKNESS, 8}, AT_NO_ATK,
       AT_NO_ATK },
    8, 520,
    3, 7, MST_NO_SPELLS, false, S_SCREECH,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_ORANGE_DEMON}, TILE_ERROR
},

{
    MONS_RED_DEVIL, '4', RED, "red devil",
    M_FIGHTER | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    13, MONS_RED_DEVIL, MONS_RED_DEVIL, MH_DEMONIC, 60,
    { {AT_HIT, AF_BARBS, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 315,
    7, 13, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_RED_DEVIL}, TILE_ERROR
},

{
    MONS_HELLWING, '4', LIGHTGREY, "hellwing",
    M_FAST_REGEN | M_FLIES,
    MR_RES_POISON,
    12, MONS_HELLWING, MONS_HELLWING, MH_DEMONIC, 60,
    { {AT_HIT, AF_SWOOP, 17}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK },
    7, 455,
    16, 10, MST_NO_SPELLS, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_HELLWING}, TILE_ERROR
},

{
    MONS_ICE_DEVIL, '4', WHITE, "ice devil",
    M_NO_FLAGS,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    11, MONS_ICE_DEVIL, MONS_ICE_DEVIL, MH_DEMONIC, 80,
    { {AT_HIT, AF_COLD, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 440,
    12, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ICE_DEVIL}, TILE_ERROR
},

{
    MONS_CHAOS_SPAWN, '4', ETC_RANDOM, "chaos spawn",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    12, MONS_CHAOS_SPAWN, MONS_CHAOS_SPAWN, MH_DEMONIC, 60,
    { {AT_RANDOM, AF_CHAOTIC, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    7, 12, MST_NO_SPELLS, false, S_RANDOM,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_CHAOS_SPAWN, TVARY_RANDOM}, TILE_ERROR
},

// middle demons ('3')
{
    MONS_SUN_DEMON, '3', YELLOW, "sun demon",
    M_FLIES,
    MR_RES_ELEC | MR_RES_POISON | MR_VUL_COLD | mrd(MR_RES_FIRE, 3),
    14, MONS_SUN_DEMON, MONS_SUN_DEMON, MH_DEMONIC, 80,
    { {AT_HIT, AF_FIRE, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    10, 12, MST_NO_SPELLS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SUN_DEMON}, TILE_ERROR
},

{
    MONS_SOUL_EATER, '3', MAGENTA, "soul eater",
    M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | MR_RES_COLD,
    13, MONS_SOUL_EATER, MONS_SOUL_EATER, MH_DEMONIC, 140,
    { {AT_HIT, AF_DRAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 605,
    18, 10, MST_SOUL_EATER, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SOUL_EATER}, TILE_ERROR
},

{
    MONS_SMOKE_DEMON, '3', LIGHTGREY, "smoke demon",
    M_INSUBSTANTIAL | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2),
    15, MONS_SMOKE_DEMON, MONS_SMOKE_DEMON, MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 8}, {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 5},
       AT_NO_ATK },
    7, 385,
    5, 9, MST_SMOKE_DEMON, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SMOKE_DEMON}, TILE_ERROR
},

{
    MONS_NEQOXEC, '3', LIGHTMAGENTA, "neqoxec",
    M_FLIES,
    MR_RES_POISON,
    12, MONS_NEQOXEC, MONS_NEQOXEC, MH_DEMONIC, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    4, 12, MST_NEQOXEC, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_NEQOXEC}, TILE_ERROR
},

{
    MONS_YNOXINUL, '3', LIGHTCYAN, "ynoxinul",
    M_FLIES,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD,
    13, MONS_YNOXINUL, MONS_YNOXINUL, MH_DEMONIC, 40,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 330,
    3, 10, MST_YNOXINUL, false, S_BELLOW,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_YNOXINUL}, TILE_ERROR
},

{
    MONS_SIXFIRHY, '3', LIGHTBLUE, "sixfirhy",
    M_NO_FLAGS,
    mrd(MR_RES_ELEC, 3),
    6, MONS_SIXFIRHY, MONS_SIXFIRHY, MH_DEMONIC, 60,
    { {AT_HIT, AF_ELEC, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 385,
    2, 20, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 40, MOVE_ENERGY(6), // speed is cut to 1/3 later
    MONUSE_OPEN_DOORS, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SIXFIRHY}, TILE_ERROR
},

// greater demons ('2')

{
    MONS_SHADOW_DEMON, '2', MAGENTA, "shadow demon",
    M_SEE_INVIS,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    13, MONS_SHADOW_DEMON, MONS_SHADOW_DEMON, MH_DEMONIC, 100,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 550,
    7, 12, MST_SHADOW_DEMON, false, S_CROAK,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SHADOW_DEMON}, TILE_ERROR
},

{
    MONS_GREEN_DEATH, '2', GREEN, "green death",
    M_SEE_INVIS,
    MR_RES_POISON,
    14, MONS_GREEN_DEATH, MONS_GREEN_DEATH, MH_DEMONIC, 160,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 715,
    5, 7, MST_GREEN_DEATH, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GREEN_DEATH}, TILE_ERROR
},

{
    MONS_BLIZZARD_DEMON, '2', LIGHTBLUE, "blizzard demon",
    M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 2) | mrd(MR_RES_ELEC, 3),
    16, MONS_BLIZZARD_DEMON, MONS_BLIZZARD_DEMON, MH_DEMONIC, 140,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    12, 660,
    10, 10, MST_BLIZZARD_DEMON, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BLIZZARD_DEMON}, TILE_ERROR
},

{
    MONS_BALRUG, '2', RED, "balrug",
    M_FIGHTER | M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    12, MONS_BALRUG, MONS_BALRUG, MH_DEMONIC, 160,
    { {AT_HIT, AF_FIRE, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    5, 12, MST_BALRUG, false, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_BALRUG}, TILE_ERROR
},

{
    MONS_CACODEMON, '2', YELLOW, "cacodemon",
    M_SEE_INVIS | M_FAST_REGEN | M_FLIES,
    MR_RES_POISON | MR_RES_ELEC,
    16, MONS_CACODEMON, MONS_CACODEMON, MH_DEMONIC, 160,
    { {AT_HIT, AF_PLAIN, 22}, {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK },
    13, 975,
    11, 10, MST_CACODEMON, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CACODEMON}, TILE_ERROR
},

{
    MONS_HELL_BEAST, '2', BROWN, "hell beast",
    M_FIGHTER,
    MR_NO_FLAGS,
    17, MONS_HELL_BEAST, MONS_HELL_BEAST, MH_DEMONIC, 20,
    { {AT_BITE, AF_PLAIN, 28}, {AT_TRAMPLE, AF_TRAMPLE, 20}, AT_NO_ATK,
       AT_NO_ATK },
    7, 840,
    5, 14, MST_NO_SPELLS, false, S_RANDOM,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_HELL_BEAST}, TILE_ERROR
},

{
    MONS_HELLION, '2', LIGHTRED, "hellion",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_DAMNATION,
    12, MONS_HELLION, MONS_HELLION, MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    5, 10, MST_HELLION, false, S_SCREAM,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HELLION}, TILE_ERROR
},

{
    MONS_REAPER, '2', LIGHTGREY, "reaper",
    M_FIGHTER | M_SEE_INVIS | M_SPEAKS,
    MR_RES_POISON | MR_RES_COLD,
    14, MONS_REAPER, MONS_REAPER, MH_DEMONIC, 100,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    15, 10, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_REAPER}, TILE_ERROR
},

{
    MONS_LOROCYPROCA, '2', BLUE, "lorocyproca",
    M_SEE_INVIS | M_INVIS,
    MR_RES_POISON | MR_RES_COLD | MR_RES_FIRE | MR_RES_ELEC,
    14, MONS_LOROCYPROCA, MONS_LOROCYPROCA, MH_DEMONIC, 140,
    { {AT_HIT, AF_ANTIMAGIC, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 770,
    10, 12, MST_NO_SPELLS, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LOROCYPROCA}, TILE_ERROR
},

{
    MONS_TORMENTOR, '2', LIGHTMAGENTA, "tormentor",
    M_SPEAKS,
    MR_RES_POISON | MR_RES_FIRE,
    10, MONS_TORMENTOR, MONS_TORMENTOR, MH_DEMONIC, 60,
    { {AT_HIT, AF_PAIN, 8}, {AT_HIT, AF_PAIN, 8}, AT_NO_ATK, AT_NO_ATK },
    7, 385,
    12, 12, MST_TORMENTOR, false, S_ROAR,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TORMENTOR}, TILE_ERROR
},

// fiends, etc. ('1')
{
    MONS_BRIMSTONE_FIEND, '1', LIGHTRED, "Brimstone Fiend",
    M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | MR_RES_DAMNATION | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    17, MONS_BRIMSTONE_FIEND, MONS_BRIMSTONE_FIEND, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 15},
       AT_NO_ATK },
    18, 990,
    15, 6, MST_BRIMSTONE_FIEND, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_BRIMSTONE_FIEND}, TILE_ERROR
},

{
    MONS_ICE_FIEND, '1', WHITE, "Ice Fiend",
    M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    17, MONS_ICE_FIEND, MONS_ICE_FIEND, MH_DEMONIC, WILL_INVULN,
    { {AT_CLAW, AF_COLD, 25}, {AT_CLAW, AF_COLD, 25}, AT_NO_ATK, AT_NO_ATK },
    18, 990,
    15, 6, MST_ICE_FIEND, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_ICE_FIEND}, TILE_ERROR
},

{
    MONS_TZITZIMITL, '1', MAGENTA, "Tzitzimitl",
    M_SEE_INVIS | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | MR_RES_ELEC,
    13, MONS_TZITZIMITL, MONS_TZITZIMITL, MH_DEMONIC, WILL_INVULN,
    { {AT_ENGULF, AF_DRAIN_SPEED, 25}, {AT_BITE, AF_DRAIN, 25},
      AT_NO_ATK, AT_NO_ATK },
    22, 1050,
    12, 16, MST_TZITZIMITL, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TZITZIMITL}, TILE_ERROR
},

{
    MONS_HELL_SENTINEL, '1', BROWN, "Hell Sentinel",
    M_SEE_INVIS,
    MR_RES_DAMNATION
        | mrd(MR_RES_FIRE | MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC, 3),
    10, MONS_HELL_SENTINEL, MONS_HELL_SENTINEL, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 40}, {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK },
    19, 1425,
    25, 3, MST_HELL_SENTINEL, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HELL_SENTINEL}, TILE_ERROR
},

{
    MONS_EXECUTIONER, '1', LIGHTGREY, "Executioner",
    M_FIGHTER | M_SEE_INVIS,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD | MR_RES_POISON,
    18, MONS_EXECUTIONER, MONS_EXECUTIONER, MH_DEMONIC, 140,
    { {AT_HIT, AF_PLAIN, 30}, {AT_HIT, AF_PLAIN, 10}, {AT_HIT, AF_PLAIN, 10},
       AT_NO_ATK },
    12, 660,
    10, 15, MST_EXECUTIONER, false, S_SCREAM,
    I_HUMAN, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EXECUTIONER}, TILE_ERROR
},

// trees and related creatures ('7')
DUMMY(MONS_ANIMATED_TREE, '7', ETC_TREE, "animated tree", TILEP_MONS_TREANT)

// non-living creatures
// golems ('8')
DUMMY(MONS_GOLEM, '9', LIGHTGREY, "golem", TILEP_MONS_IRON_GOLEM)

{
    MONS_SALTLING, '9', WHITE, "saltling",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    10, MONS_GOLEM, MONS_SALTLING, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 270,
    15, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SALTLING}, TILE_ERROR
},

{
    MONS_PEACEKEEPER, '9', YELLOW, "peacekeeper",
    M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_GOLEM, MONS_PEACEKEEPER, MH_NONLIVING, WILL_INVULN,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK },
    12, 450,
    20, 3, MST_PEACEKEEPER, false, S_ROAR,
    I_BRAINLESS, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_PEACEKEEPER}, TILE_ERROR
},

{
    MONS_IRON_GOLEM, '9', CYAN, "iron golem",
    M_NO_FLAGS,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    10, MONS_GOLEM, MONS_IRON_GOLEM, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 50}, {AT_HIT, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK },
    15, 3000,
    25, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 7, ATTACK_ENERGY(7),
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IRON_GOLEM}, TILE_ERROR
},

{
    MONS_CRYSTAL_GUARDIAN, '9', GREEN, "crystal guardian",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    10, MONS_GOLEM, MONS_CRYSTAL_GUARDIAN, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 640,
    22, 0, MST_CRYSTAL_GUARDIAN, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CRYSTAL_GUARDIAN}, TILE_ERROR
},

{
    MONS_TOENAIL_GOLEM, '9', RED, "toenail golem",
    M_NO_FLAGS,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_GOLEM, MONS_TOENAIL_GOLEM, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 585,
    8, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TOENAIL_GOLEM}, TILE_ERROR
},

{
    MONS_ELECTRIC_GOLEM, '9', LIGHTBLUE, "electric golem",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_SPEAKS,
    mrd(MR_RES_ELEC, 3) | MR_RES_FIRE | MR_RES_COLD,
    12, MONS_GOLEM, MONS_ELECTRIC_GOLEM, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_ELEC, 15}, {AT_HIT, AF_ELEC, 15}, {AT_HIT, AF_PLAIN, 15},
      {AT_HIT, AF_PLAIN, 15} },
    15, 1350,
    5, 20, MST_ELECTRIC_GOLEM, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 16, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ELECTRIC_GOLEM}, TILE_ERROR
},

{
    MONS_USHABTI, '9', BROWN, "ushabti",
    M_FIGHTER | M_SEE_INVIS,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_GOLEM, MONS_USHABTI, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 455,
    9, 6, MST_USHABTI, false, S_SILENT,
    I_HUMAN, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_MISC,
    {TILEP_MONS_USHABTI}, TILE_ERROR
},

{
    MONS_GUARDIAN_GOLEM, '9', LIGHTGREEN, "guardian golem",
    M_NO_EXP_GAIN,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    10, MONS_GOLEM, MONS_GUARDIAN_GOLEM, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 400,
    10, 2, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GUARDIAN_GOLEM}, TILE_ERROR
},

{
    MONS_SPELLFORGED_SERVITOR, '9', LIGHTMAGENTA, "spellforged servitor",
    M_NO_POLY_TO | M_FLIES | M_NO_EXP_GAIN,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC,
    10, MONS_GOLEM, MONS_SPELLFORGED_SERVITOR, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 800,
    10, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, SPELL_ENERGY(18),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SPELLFORGED_SERVITOR}, TILE_ERROR
},

// statues and statue-like things (also '8')
{
    MONS_ICE_STATUE, '8', LIGHTBLUE, "ice statue",
    M_STATIONARY | M_SPEAKS,
    MR_VUL_FIRE | mrd(MR_RES_COLD, 3) | MR_RES_ELEC | MR_RES_PETRIFY,
    10, MONS_STATUE, MONS_ICE_STATUE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 720,
    12, 1, MST_ICE_STATUE, false, S_SILENT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ICE_STATUE}, TILE_ERROR
},

{
    MONS_OBSIDIAN_STATUE, '8', MAGENTA, "obsidian statue",
    M_STATIONARY | M_SPEAKS | M_SEE_INVIS,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC
        | MR_RES_PETRIFY,
    10, MONS_STATUE, MONS_OBSIDIAN_STATUE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 700,
    12, 1, MST_OBSIDIAN_STATUE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_OBSIDIAN_STATUE}, TILE_ERROR
},

{
    MONS_ORANGE_STATUE, '8', LIGHTRED, "orange crystal statue",
    M_STATIONARY | M_SPEAKS | M_SEE_INVIS,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC | MR_RES_PETRIFY,
    10, MONS_STATUE, MONS_ORANGE_STATUE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 700,
    12, 1, MST_ORANGE_CRYSTAL_STATUE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ORANGE_STATUE}, TILE_ERROR
},

{ // always redefined
    MONS_STATUE, '8', LIGHTGREY, "statue",
    M_STATIONARY | M_SPEAKS | M_ARCHER | M_PREFER_RANGED | M_NO_POLY_TO,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC | MR_RES_PETRIFY,
    10, MONS_STATUE, MONS_STATUE, MH_NONLIVING, WILL_INVULN,
    { {AT_WEAP_ONLY, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 720,
    12, 1, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_OBSIDIAN_STATUE}, TILE_ERROR
},

{
    MONS_TRAINING_DUMMY, '8', LIGHTGREY, "training dummy",
    M_STATIONARY,
    MR_RES_PETRIFY,
    10, MONS_TRAINING_DUMMY, MONS_TRAINING_DUMMY, MH_NONLIVING, WILL_INVULN,
    { {AT_WEAP_ONLY, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 60,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TRAINING_DUMMY}, TILE_ERROR
},

{
    MONS_PILLAR_OF_SALT, '8', WHITE, "pillar of salt",
    M_NOT_DANGEROUS | M_STATIONARY,
    MR_NO_FLAGS,
    10, MONS_PILLAR_OF_SALT, MONS_PILLAR_OF_SALT, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    1, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_PILLAR_OF_SALT}, TILE_ERROR
},

{
    MONS_BLOCK_OF_ICE, '8', ETC_ICE, "block of ice",
    M_NOT_DANGEROUS | M_STATIONARY,
    mrd(MR_RES_COLD, 3) | MR_VUL_FIRE,
    10, MONS_BLOCK_OF_ICE, MONS_BLOCK_OF_ICE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 285,
    15, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_BLOCK_OF_ICE, TVARY_MOD}, TILE_ERROR
},

{
    MONS_DIAMOND_OBELISK, '8', WHITE, "diamond obelisk",
    M_STATIONARY | M_NOT_DANGEROUS | M_NO_POLY_TO,
    MR_RES_PETRIFY,
    10, MONS_DIAMOND_OBELISK, MONS_DIAMOND_OBELISK, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    8, 96000,
    12, 1, MST_NO_SPELLS, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_DIAMOND_OBELISK}, TILE_ERROR
},

{
    MONS_LIGHTNING_SPIRE, '8', ETC_ELECTRICITY, "lightning spire",
    M_STATIONARY | M_NO_POLY_TO,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | mrd(MR_RES_ELEC, 3) | MR_RES_PETRIFY,
    10, MONS_LIGHTNING_SPIRE, MONS_LIGHTNING_SPIRE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 210,
    13, 3, MST_ZAPPER, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_LIGHTNING_SPIRE}, TILE_ERROR
},

{
    MONS_STRANGE_MACHINE, '8', ETC_SILVER, "strange machine",
    M_STATIONARY | M_NO_POLY_TO,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC | MR_RES_PETRIFY,
    10, MONS_STRANGE_MACHINE, MONS_STRANGE_MACHINE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    21, 900,
    12, 1, MST_STRANGE_MACHINE, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_MISC,
    {TILEP_MONS_STRANGE_MACHINE}, TILE_ERROR
},


// gargoyles ('9')
{
    MONS_GARGOYLE, '9', LIGHTGREY, "gargoyle",
    M_FLIES,
    MR_RES_ELEC | MR_RES_PETRIFY,
    26, MONS_GOLEM, MONS_GARGOYLE, MH_NONLIVING, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 270,
    18, 6, MST_GARGOYLE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED_TAILED,
    {TILEP_MONS_GARGOYLE}, TILE_ERROR
},

{
    MONS_WAR_GARGOYLE, '9', LIGHTCYAN, "war gargoyle",
    M_SEE_INVIS | M_FIGHTER | M_FLIES,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD | MR_RES_PETRIFY,
    18, MONS_GOLEM, MONS_WAR_GARGOYLE, MH_NONLIVING, 100,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 520,
    22, 4, MST_SPLINTER_BREATH, false, S_SILENT,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED_TAILED,
    {TILEP_MONS_WAR_GARGOYLE}, TILE_ERROR
},

{
    MONS_MOLTEN_GARGOYLE, '9', LIGHTRED, "molten gargoyle",
    M_FLIES,
    MR_RES_ELEC | mrd(MR_RES_FIRE, 3) | MR_RES_PETRIFY,
    18, MONS_GOLEM, MONS_MOLTEN_GARGOYLE, MH_NONLIVING, 60,
    { {AT_HIT, AF_FIRE, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    7, 315,
    14, 7, MST_MOLTEN_GARGOYLE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED_TAILED,
    {TILEP_MONS_MOLTEN_GARGOYLE}, TILE_ERROR
},

{
    MONS_NARGUN, '9', BLUE, "nargun",
    M_SEE_INVIS | M_FIGHTER,
    MR_RES_ELEC | mrd(MR_RES_COLD, 3) | MR_RES_PETRIFY,
    10, MONS_GOLEM, MONS_NARGUN, MH_NONLIVING, 100,
    { {AT_HIT, AF_VULN, 40}, {AT_BITE, AF_COLD, 30}, AT_NO_ATK, AT_NO_ATK },
    22, 540,
    25, 4, MST_NARGUN, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_BLOB,
    {TILEP_MONS_NARGUN}, TILE_ERROR
},

// major demons ('&')
// Random demon in pan - only one per level. Stats are stored in ghost struct.
{
    MONS_PANDEMONIUM_LORD, '&', COLOUR_UNDEF, "pandemonium lord",
    // See invis is also set in ghost.cc
    M_FIGHTER | M_SPEAKS | M_SEE_INVIS | M_GHOST_DEMON | M_TALL_TILE,
    MR_RES_POISON,
    14, MONS_PANDEMONIUM_LORD, MONS_PANDEMONIUM_LORD, MH_DEMONIC, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    19, 1710,
    1, 2, MST_GHOST, false, S_DEMON_TAUNT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PANDEMONIUM_LORD}, TILE_ERROR
},

DUMMY(MONS_HELL_LORD, '&', COLOUR_UNDEF, "hell lord", TILEP_MONS_PROGRAM_BUG)

// explodey things / orb of fire ('*')
{
    MONS_BALL_LIGHTNING, '*', LIGHTCYAN, "ball lightning",
    M_INSUBSTANTIAL | M_CONJURED | M_FLIES | M_NO_EXP_GAIN,
    mrd(MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD, 3),
    20, MONS_BALL_LIGHTNING, MONS_BALL_LIGHTNING, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_BALL_LIGHTNING}, TILE_ERROR
},

{
    MONS_BALLISTOMYCETE_SPORE, '*', GREEN, "ballistomycete spore",
    M_FLIES | M_NO_EXP_GAIN,
    MR_RES_POISON,
    10, MONS_FUNGUS, MONS_BALLISTOMYCETE_SPORE, MH_PLANT, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ORB,
    {TILEP_MONS_BALLISTOMYCETE_SPORE}, TILE_ERROR
},

{
    MONS_LOST_SOUL, '*', LIGHTGREEN, "lost soul",
    M_INSUBSTANTIAL | M_MAINTAIN_RANGE | M_FLIES | M_NO_EXP_GAIN | M_SEE_INVIS,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    2, MONS_LOST_SOUL, MONS_LOST_SOUL, MH_UNDEAD, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 250,
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ORB,
    {TILEP_MONS_LOST_SOUL}, TILE_ERROR
},

{
    MONS_LURKING_HORROR, '*', BLUE, "lurking horror",
    M_INSUBSTANTIAL | M_FLIES | M_NO_EXP_GAIN,
    MR_NO_FLAGS,
    10, MONS_LURKING_HORROR, MONS_LURKING_HORROR, MH_NONLIVING, 10,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ORB,
    {TILEP_MONS_LURKING_HORROR}, TILE_ERROR
},

{
    MONS_ORB_OF_FIRE, '*', RED, "orb of fire",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    mrd(MR_RES_FIRE | MR_RES_ELEC, 3) | MR_RES_COLD,
    13, MONS_ORB_OF_FIRE, MONS_ORB_OF_FIRE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    30, 1500,
    20, 20, MST_ORB_OF_FIRE, false, S_SILENT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_ORB_OF_FIRE}, TILE_ERROR
},

{
    MONS_CREEPING_INFERNO, '*', LIGHTRED, "creeping inferno",
    M_INSUBSTANTIAL | M_FLIES | M_NO_EXP_GAIN,
    mrd(MR_RES_FIRE, 3),
    10, MONS_CREEPING_INFERNO, MONS_CREEPING_INFERNO, MH_NONLIVING, 10,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 10,
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_SMALL, MON_SHAPE_ORB,
    {TILEP_MONS_CREEPING_INFERNO}, TILE_ERROR
},

{ // not an actual monster, used by a spell
    MONS_ORB_OF_DESTRUCTION, '*', WHITE, "orb of destruction",
    M_INSUBSTANTIAL | M_NOT_DANGEROUS | M_NO_POLY_TO | M_PROJECTILE | M_FLIES,
    mrd(MR_RES_COLD | MR_RES_ELEC | MR_RES_ACID | MR_RES_FIRE, 3)
        | MR_RES_DAMNATION | MR_RES_STICKY_FLAME,
    0, MONS_ORB_OF_DESTRUCTION, MONS_ORB_OF_DESTRUCTION,
        MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 50000, /* unkillable */
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_ORB_OF_DESTRUCTION, TVARY_RANDOM}, TILE_ERROR
},

{ // not an actual monster, used by a spell
    MONS_FOXFIRE, 'v', LIGHTRED, "foxfire",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_NOT_DANGEROUS | M_NO_POLY_TO
        | M_CONJURED | M_FLIES,
    mrd(MR_RES_FIRE, 3) | MR_RES_STICKY_FLAME,
    0, MONS_FOXFIRE, MONS_FOXFIRE, MH_NONLIVING, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10, /* unkillable */
    0, 10, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 50, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_FOXFIRE}, TILE_ERROR
},

{ // not an actual monster, used by a spell
    MONS_FULMINANT_PRISM, '*', ETC_MAGIC, "fulminant prism",
    M_NO_POLY_TO | M_STATIONARY | M_CONJURED | M_NO_EXP_GAIN
        | M_FLIES,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, MONS_FULMINANT_PRISM, MONS_FULMINANT_PRISM, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 150,
    3, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_FULMINANT_PRISM, TVARY_RANDOM}, TILE_ERROR
},

{
    MONS_WRETCHED_STAR, '*', MAGENTA, "wretched star",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_FLIES,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    13, MONS_WRETCHED_STAR, MONS_WRETCHED_STAR, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 700,
    10, 10, MST_WRETCHED_STAR, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(8),
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_WRETCHED_STAR}, TILE_ERROR
},

{
    MONS_BATTLESPHERE, '*', ETC_MAGIC, "battlesphere",
    M_SEE_INVIS | M_INSUBSTANTIAL | M_NO_EXP_GAIN | M_NO_POLY_TO
        | M_MAINTAIN_RANGE | M_AVATAR | M_FLIES | M_UNBLINDABLE,
    mrd(MR_RES_COLD | MR_RES_FIRE, 2) | mrd(MR_RES_ELEC, 3)
        | MR_RES_ACID | MR_RES_STICKY_FLAME,
    0, MONS_BATTLESPHERE, MONS_BATTLESPHERE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 150,
    0, 5, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 30, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LITTLE, MON_SHAPE_ORB,
    {TILEP_MONS_BATTLESPHERE}, TILE_ERROR
},

// other symbols
{
    MONS_DEATH_COB, 'z', YELLOW, "death cob",
    M_SPEAKS,
    MR_RES_COLD,
    10, MONS_DEATH_COB, MONS_DEATH_COB, MH_UNDEAD, 40,
    { {AT_HIT, AF_DRAIN_SPEED, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 910,
    10, 15, MST_NO_SPELLS, false, S_MOAN,
    I_HUMAN, HT_LAND, 25, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_TINY, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DEATH_COB}, TILE_ERROR
},

// non-human uniques
// "A"ngels.
{
    MONS_MENNAS, 'A', LIGHTCYAN, "Mennas",
    M_FIGHTER | M_SPEAKS | M_UNIQUE | M_MALE | M_FLIES,
    MR_RES_POISON | MR_RES_ELEC,
    18, MONS_ANGEL, MONS_ANGEL, MH_HOLY, 160,
    { {AT_HIT, AF_PLAIN, 30}, {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK,
       AT_NO_ATK },
    19, 1520,
    15, 28, MST_MENNAS, false, S_SILENT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_MENNAS}, TILE_ERROR
},

//  "a"ncients.
{
    MONS_ZENATA, 'a' /*should be @?*/, LIGHTGRAY, "Zenata",
    M_SPEAKS | M_WARM_BLOOD | M_UNIQUE | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 1800,
    10, 10, MST_ZENATA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ZENATA}, TILE_ERROR
},

// "c"entaurs.
{
    MONS_NESSOS, 'c', MAGENTA, "Nessos",
    M_UNIQUE | M_WARM_BLOOD | M_ARCHER | M_SPEAKS | M_GENDER_NEUTRAL,
    MR_NO_FLAGS,
    18, MONS_CENTAUR, MONS_CENTAUR, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 13}, {AT_KICK, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    9, 720,
    4, 8, MST_NESSOS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_CENTAUR,
    {TILEP_MONS_NESSOS}, TILE_ERROR
},

// "C"yclopes and giants.
{
    MONS_CHUCK, 'C', WHITE, "Chuck",
    M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_MALE,
    MR_NO_FLAGS,
    10, MONS_GIANT, MONS_STONE_GIANT, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1170,
    14, 2, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CHUCK}, TILE_ERROR
},

{
    MONS_POLYPHEMUS, 'C', BLUE, "Polyphemus",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    10, MONS_GIANT, MONS_CYCLOPS, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 45}, {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    16, 1520,
    10, 3, MST_POLYPHEMUS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_POLYPHEMUS}, TILE_ERROR
},

{
    MONS_ANTAEUS, 'C', WHITE, "Antaeus",
    M_UNIQUE | M_WARM_BLOOD | M_FIGHTER | M_SEE_INVIS | M_MALE
        | M_SPEAKS | M_TALL_TILE,
    mrd(MR_RES_ELEC, 3) | mrd(MR_RES_COLD, 2) | MR_VUL_FIRE,
    10, MONS_GIANT, MONS_TITAN, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_COLD, 75}, {AT_HIT, AF_COLD, 30}, AT_NO_ATK, AT_NO_ATK },
    22, 6820,
    28, 4, MST_ANTAEUS, false, S_DEMON_TAUNT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ANTAEUS}, TILE_ERROR
},

// "d"raconians.
{
    MONS_TIAMAT, 'd', COLOUR_UNDEF, "Tiamat",
    M_UNIQUE | M_SEE_INVIS | M_COLD_BLOOD | M_SPEAKS | M_FEMALE | M_FLIES,
    MR_RES_POISON,
    10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 60}, {AT_TAIL_SLAP, AF_PLAIN, 45}, AT_NO_ATK,
       AT_NO_ATK },
    22, 3850,
    // Gets her breath in
    30, 10, MST_NO_SPELLS, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, // bigtile!
        MON_SHAPE_HUMANOID_WINGED_TAILED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_BAI_SUZHEN, 'd', LIGHTBLUE, "Bai Suzhen",
    M_UNIQUE | M_SEE_INVIS | M_COLD_BLOOD | M_SPEAKS | M_FEMALE,
    mrd(MR_RES_ELEC, 3) | MR_RES_COLD | MR_RES_POISON,
    16, MONS_DRAGON, MONS_STORM_DRAGON, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 24}, {AT_TAIL_SLAP, AF_PLAIN, 14}, AT_NO_ATK,
       AT_NO_ATK },
    20, 1850,
    14, 8, MST_BAI_SUZHEN, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID_TAILED,
    {TILEP_MONS_BAI_SUZHEN}, TILE_ERROR
},

// "D"ragons and hydras.
{
    MONS_XTAHUA, 'D', RED, "Xtahua",
    M_UNIQUE | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_GENDER_NEUTRAL
        | M_CRASH_DOORS | M_FLIES,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2) | MR_VUL_COLD,
    18, MONS_DRAGON, MONS_FIRE_DRAGON, MH_NATURAL, 180,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 17},
      {AT_TRAMPLE, AF_TRAMPLE, 20}, AT_NO_ATK },
    20, 1330,
    15, 7, MST_XTAHUA, true, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_XTAHUA}, TILE_ERROR
},

{
    MONS_LERNAEAN_HYDRA, 'D', YELLOW, "the Lernaean hydra",
    M_UNIQUE | M_COLD_BLOOD | M_FAST_REGEN | M_TALL_TILE,
    MR_RES_POISON,
    11, MONS_HYDRA, MONS_HYDRA, MH_NATURAL, 120,
    { {AT_BITE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    30, 1500,
    0, 5, MST_NO_SPELLS, true, S_LOUD_ROAR,
    I_ANIMAL, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_PROGRAM_BUG}, TILE_ERROR
},

{
    MONS_SERPENT_OF_HELL, 'D', ETC_FIRE, "the Serpent of Hell",
    M_SEE_INVIS | M_UNIQUE | M_CRASH_DOORS | M_FLIES | M_TALL_TILE,
    MR_RES_POISON | MR_RES_DAMNATION | mrd(MR_RES_FIRE, 3),
    21, MONS_DRAGON, MONS_SERPENT_OF_HELL, MH_DEMONIC, 180,
    { {AT_BITE, AF_FIRE, 35}, {AT_CLAW, AF_PLAIN, 15},
      {AT_TRAMPLE, AF_TRAMPLE, 15}, AT_NO_ATK },
    17, 2805,
    16, 12, MST_SERPENT_OF_HELL_GEH, false, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SERPENT_OF_HELL_GEHENNA}, TILE_ERROR
},

{
    MONS_SERPENT_OF_HELL_COCYTUS, 'D', ETC_ICE, "the Serpent of Hell",
    M_SEE_INVIS | M_UNIQUE | M_CRASH_DOORS | M_FLIES | M_TALL_TILE,
    MR_RES_POISON | mrd(MR_RES_COLD, 3),
    21, MONS_DRAGON, MONS_SERPENT_OF_HELL, MH_DEMONIC, 180,
    { {AT_BITE, AF_COLD, 35}, {AT_CLAW, AF_PLAIN, 15},
      {AT_TRAMPLE, AF_TRAMPLE, 15}, AT_NO_ATK },
    17, 2805,
    20, 12, MST_SERPENT_OF_HELL_COC, false, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SERPENT_OF_HELL_COCYTUS}, TILE_ERROR
},

{
    MONS_SERPENT_OF_HELL_DIS, 'D', ETC_IRON, "the Serpent of Hell",
    M_SEE_INVIS | M_UNIQUE | M_CRASH_DOORS | M_TALL_TILE,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    16, MONS_DRAGON, MONS_SERPENT_OF_HELL, MH_DEMONIC, 180,
    { {AT_BITE, AF_REACH, 35}, {AT_CLAW, AF_PLAIN, 25},
      {AT_TRAMPLE, AF_TRAMPLE, 25}, {AT_TAIL_SLAP, AF_PLAIN, 15} },
    17, 3230,
    30, 8, MST_SERPENT_OF_HELL_DIS, false, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SERPENT_OF_HELL_DIS}, TILE_ERROR
},

{
    MONS_SERPENT_OF_HELL_TARTARUS, 'D', ETC_DEATH, "the Serpent of Hell",
    M_SEE_INVIS | M_UNIQUE | M_CRASH_DOORS | M_FLIES | M_TALL_TILE,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    18, MONS_DRAGON, MONS_SERPENT_OF_HELL, MH_DEMONIC, 180,
    { {AT_BITE, AF_DRAIN, 35}, {AT_CLAW, AF_PLAIN, 15},
      {AT_TRAMPLE, AF_TRAMPLE, 15}, AT_NO_ATK },
    17, 3230,
    16, 12, MST_SERPENT_OF_HELL_TAR, false, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED_WINGED,
    {TILEP_MONS_SERPENT_OF_HELL_TARTARUS}, TILE_ERROR
},

{
    MONS_BAI_SUZHEN_DRAGON, 'D', ETC_ELECTRICITY, "Bai Suzhen",
    M_UNIQUE | M_SEE_INVIS | M_COLD_BLOOD | M_SPEAKS | M_FEMALE | M_FLIES
        | M_THUNDER_RING,
    mrd(MR_RES_ELEC, 3) | MR_RES_COLD | MR_RES_POISON,
    18, MONS_DRAGON, MONS_STORM_DRAGON, MH_NATURAL, 100,
    { {AT_BITE, AF_PLAIN, 30}, {AT_CLAW, AF_PLAIN, 16},
      {AT_TRAMPLE, AF_TRAMPLE, 16}, AT_NO_ATK },
    20, 1850,
    22, 4, MST_BAI_SUZHEN_DRAGON, false, S_LOUD_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_GIANT, MON_SHAPE_SNAKE,
    {TILEP_MONS_BAI_SUZHEN_DRAGON}, TILE_ERROR
},

// "e"lves
{
    MONS_DUVESSA, 'e', BLUE, "Duvessa",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_ELF, MONS_ELF, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 360,
    2, 9, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DUVESSA}, TILE_ERROR
},

{
    MONS_DOWAN, 'e', RED, "Dowan",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_ELF, MONS_ELF, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 240,
    0, 13, MST_DOWAN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DOWAN}, TILE_ERROR
},

{
    MONS_FANNAR, 'e', LIGHTBLUE, "Fannar",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_GENDER_NEUTRAL,
    MR_NO_FLAGS,
    16, MONS_ELF, MONS_ELF, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 800,
    4, 15, MST_FANNAR, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FANNAR}, TILE_ERROR
},

// "F"rogs.
{
    MONS_PRINCE_RIBBIT, 'F', LIGHTCYAN, "Prince Ribbit",
    M_UNIQUE | M_COLD_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    12, MONS_FROG, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 390,
    0, 16, MST_BLINKER, true, S_CROAK,
    I_HUMAN, HT_AMPHIBIOUS, 14, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_QUADRUPED_TAILLESS,
    {TILEP_MONS_PRINCE_RIBBIT}, TILE_ERROR
},

// "g"oblins, gnolls, and dwarves.
{
    MONS_IJYB, 'g', BLUE, "Ijyb",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE | M_ALWAYS_WAND,
    MR_NO_FLAGS,
    8, MONS_GOBLIN, MONS_GOBLIN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 240,
    2, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IJYB}, TILE_ERROR
},

{
    MONS_ROBIN, 'g', LIGHTCYAN, "Robin",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_GENDER_NEUTRAL,
    MR_NO_FLAGS,
    10, MONS_GOBLIN, MONS_HOBGOBLIN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 160,
    1, 8, MST_ROBIN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ROBIN}, TILE_ERROR
},

{
    MONS_GRUM, 'g', LIGHTRED, "Grum",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    19, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 400,
    2, 9, MST_NO_SPELLS, true, S_GROWL,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GRUM}, TILE_ERROR
},

{
    MONS_CRAZY_YIUF, 'g', COLOUR_UNDEF, "Crazy Yiuf",
    M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_MALE,
    MR_NO_FLAGS,
    10, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 195,
    2, 9, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CRAZY_YIUF}, TILE_ERROR
},

{
    MONS_JORGRUN, 'g', LIGHTMAGENTA, "Jorgrun",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    18, MONS_DWARF, MONS_DWARF, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 1200,
    2, 15, MST_JORGRUN, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JORGRUN}, TILE_ERROR
},

// "h" - carnivorous quadrupeds
{
    MONS_NATASHA, 'h', MAGENTA, "Natasha",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_FEMALE,
    MR_NO_FLAGS,
    10, MONS_FELID, MONS_FELID, MH_NATURAL, 20,
    { {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 150,
    2, 12, MST_NATASHA, true, S_HISS,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(8),
    MONUSE_STARTING_EQUIPMENT, SIZE_LITTLE, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_NATASHA}, TILE_ERROR
},

// "H"ybrids.
{
    MONS_ARACHNE, 'H', LIGHTCYAN, "Arachne",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FEMALE | M_WEB_IMMUNE,
    MR_NO_FLAGS, // no rPois- (breathes through the human half)
    10, MONS_SPIDER, MONS_ARACHNE, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    17, 2040,
    3, 10, MST_ARACHNE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_ARACHNID,
    {TILEP_MONS_ARACHNE}, TILE_CORPSE_ARACHNE
},

{
    MONS_ASTERION, 'H', LIGHTBLUE, "Asterion",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_UNIQUE | M_MALE,
    MR_NO_FLAGS,
    15, MONS_MINOTAUR, MONS_MINOTAUR, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 35}, {AT_GORE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    15, 1275,
    4, 4, MST_ASTERION, true, S_BELLOW,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ASTERION}, TILE_ERROR
},

// Spr"i"ggans.
{
    MONS_AGNES, 'i', LIGHTCYAN, "Agnes",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_FEMALE,
    MR_NO_FLAGS,
    20, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 990,
    0, 20, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 18, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_AGNES}, TILE_ERROR
},

{
    MONS_THE_ENCHANTRESS, 'i', LIGHTMAGENTA, "the Enchantress",
    M_WARM_BLOOD | M_SPEAKS | M_SEE_INVIS | M_UNIQUE | M_FEMALE,
    MR_NO_FLAGS,
    35, MONS_SPRIGGAN, MONS_SPRIGGAN, MH_NATURAL, 160,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 975,
    1, 30, MST_THE_ENCHANTRESS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_THE_ENCHANTRESS}, TILE_ERROR
},

// "J"ellies.
{
    MONS_ROYAL_JELLY, 'J', YELLOW, "the Royal Jelly",
    M_SEE_INVIS | M_UNBLINDABLE | M_ACID_SPLASH | M_UNIQUE | M_EAT_DOORS
        | M_TALL_TILE | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    20, MONS_JELLY, MONS_JELLY, MH_NATURAL, 180,
    { {AT_HIT, AF_ACID, 50}, {AT_HIT, AF_ACID, 30}, AT_NO_ATK, AT_NO_ATK },
    21, 2310,
    8, 4, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_BLOB,
    {TILEP_MONS_ROYAL_JELLY}, TILE_ERROR
},

{
    MONS_DISSOLUTION, 'J', LIGHTMAGENTA, "Dissolution",
    M_UNIQUE | M_SEE_INVIS | M_UNBLINDABLE | M_ACID_SPLASH | M_BURROWS
        | M_SPEAKS | M_FAST_REGEN | M_GENDER_NEUTRAL | M_EAT_DOORS
        | M_WEB_IMMUNE,
    MR_RES_POISON | mrd(MR_RES_ACID, 3),
    60, MONS_JELLY, MONS_JELLY, MH_NATURAL, 120,
    { {AT_HIT, AF_ACID, 50}, {AT_HIT, AF_ACID, 30}, AT_NO_ATK, AT_NO_ATK },
    16, 1760,
    10, 1, MST_DISSOLUTION, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_BLOB,
    {TILEP_MONS_DISSOLUTION}, TILE_ERROR
},

// "K"obolds.
{
    MONS_SONJA, 'K', RED, "Sonja",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    40, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 9}, {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 5},
       AT_NO_ATK },
    6, 300,
    2, 24, MST_SONJA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_SMALL, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SONJA}, TILE_ERROR
},

{
    // XP modifier is very high to compensate for 4 created-friendly humans
    MONS_PIKEL, 'K', BLUE, "Pikel",
    M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_MALE,
    MR_NO_FLAGS,
    32, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 390,
    4, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PIKEL}, TILE_ERROR
},

// "L"iches.
{
    // May be re-spawned after his death.
    MONS_BORIS, 'L', RED, "Boris",
    M_UNIQUE | M_SEE_INVIS | M_SPEAKS | M_MALE,
    mrd(MR_RES_COLD, 2) | MR_RES_ELEC,
    15, MONS_LICH, MONS_LICH, MH_UNDEAD, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 25}, {AT_TOUCH, AF_DRAIN, 15}, AT_NO_ATK,
       AT_NO_ATK },
    22, 1540,
    12, 10, MST_BORIS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BORIS}, TILE_ERROR
},

{
    MONS_JOSEPHINA, 'L', LIGHTBLUE, "Josephina",
    M_UNIQUE | M_SEE_INVIS | M_SPEAKS | M_FEMALE,
    mrd(MR_RES_COLD, 3),
    20, MONS_LICH, MONS_LICH, MH_UNDEAD, WILL_INVULN,
    { {AT_HIT, AF_COLD, 27}, {AT_TOUCH, AF_DRAIN, 15}, AT_NO_ATK,
       AT_NO_ATK },
    21, 1500,
    10, 21, MST_JOSEPHINA, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JOSEPHINA}, TILE_ERROR
},

// "M"ummies.
{
    MONS_MENKAURE, 'M', MAGENTA, "Menkaure",
    M_UNIQUE | M_SPEAKS | M_SEE_INVIS | M_MALE,
    MR_VUL_FIRE | MR_RES_COLD,
    48, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 20,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 240,
    3, 6, MST_MENKAURE, false, S_SHOUT,
    I_HUMAN, HT_LAND, 8, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MENKAURE}, TILE_ERROR
},

{
    MONS_KHUFU, 'M', LIGHTRED, "Khufu",
    M_SEE_INVIS | M_SPEAKS | M_UNIQUE | M_MALE,
    MR_RES_COLD | MR_RES_ELEC,
    20, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, 160,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 2430,
    10, 6, MST_KHUFU, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KHUFU}, TILE_ERROR
},

// "m"erfolk.
{
    MONS_ILSUIW, 'm', LIGHTGREEN, "Ilsuiw",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    10, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1520,
    5, 18, MST_ILSUIW, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, SWIM_ENERGY(6),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ILSUIW, TVARY_WATER}, TILE_ERROR
},

// "N"agas.
{
    MONS_VASHNIA, 'N', LIGHTCYAN, "Vashnia",
    M_SEE_INVIS | M_WARM_BLOOD | M_FEMALE
        | M_ARCHER | M_PREFER_RANGED | M_SPEAKS | M_UNIQUE,
    MR_RES_POISON,
    27, MONS_NAGA, MONS_NAGA, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 25}, {AT_CONSTRICT, AF_CRUSH, 7}, AT_NO_ATK, AT_NO_ATK },
    16, 1600,
    6, 18, MST_VASHNIA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(14),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_NAGA,
    {TILEP_MONS_VASHNIA}, TILE_ERROR
},

// "O"gres.
{
    MONS_EROLCHA, 'O', LIGHTBLUE, "Erolcha",
    M_UNIQUE | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    50, MONS_OGRE, MONS_OGRE, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 540,
    3, 7, MST_WIZARD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EROLCHA}, TILE_ERROR
},

{
    MONS_LODUL, 'O', CYAN, "Lodul",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    25, MONS_OGRE, MONS_OGRE, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 700,
    3, 7, MST_LODUL, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LODUL}, TILE_ERROR
},

// "o"rcs.
{
    MONS_BLORK_THE_ORC, 'o', BROWN, "Blork the orc",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_ORC, MONS_ORC, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 315,
    0, 9, MST_BLORK, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_BLORK_THE_ORC}, TILE_ERROR
},

{
    MONS_URUG, 'o', RED, "Urug",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_ORC, MONS_ORC, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 675,
    2, 13, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_URUG}, TILE_ERROR
},

{
    MONS_NERGALLE, 'o', WHITE, "Nergalle",
    M_UNIQUE | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    12, MONS_ORC, MONS_ORC, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 600,
    9, 11, MST_NERGALLE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_NERGALLE}, TILE_ERROR
},

{
    MONS_SAINT_ROKA, 'o', LIGHTBLUE, "Saint Roka",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_GENDER_NEUTRAL,
    MR_NO_FLAGS,
    15, MONS_ORC, MONS_ORC, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1980,
    3, 10, MST_SAINT_ROKA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SAINT_ROKA}, TILE_ERROR
},

// Tengu.
{
    MONS_SOJOBO, 'Q', LIGHTGREEN, "Sojobo",
    M_FIGHTER | M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_FEMALE
        | M_FLIES,
    MR_NO_FLAGS,
    20, MONS_TENGU, MONS_TENGU, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 28}, {AT_PECK, AF_PLAIN, 14}, {AT_CLAW, AF_PLAIN, 14},
       AT_NO_ATK },
    20, 1500,
    2, 24, MST_SOJOBO, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, MOVE_ENERGY(9),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SOJOBO}, TILE_ERROR
},

// Rakshasas and demons.
{
    MONS_AZRAEL, 'R', LIGHTRED, "Azrael",
    M_UNIQUE | M_SPEAKS | M_MALE | M_FLIES,
    MR_RES_POISON | MR_RES_DAMNATION | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    12, MONS_EFREET, MONS_EFREET, MH_DEMONIC, 40,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    11, 880,
    10, 5, MST_AZRAEL, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_AZRAEL}, TILE_ERROR
},

{
    MONS_MARA, 'R', LIGHTMAGENTA, "Mara",
    M_SEE_INVIS | M_SPEAKS | M_UNIQUE | M_MALE,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2),
    35, MONS_RAKSHASA, MONS_RAKSHASA, MH_DEMONIC, 140,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1440,
    10, 14, MST_MARA, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MARA}, TILE_ERROR
},

// "S"nakes and guardian serpents.
{
    MONS_AIZUL, 'S', LIGHTMAGENTA, "Aizul",
    M_SEE_INVIS | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE | M_FEMALE,
    MR_RES_POISON,
    10, MONS_GUARDIAN_SERPENT, MONS_GUARDIAN_SERPENT, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 1400,
    8, 18, MST_AIZUL, true, S_SHOUT,
    I_HUMAN, HT_LAND, 15, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_SNAKE,
    {TILEP_MONS_AIZUL}, TILE_ERROR
},

// "T"rolls.
{
    // Snorg can go berserk.
    MONS_SNORG, 'T', LIGHTGREEN, "Snorg",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN | M_FEMALE,
    MR_NO_FLAGS,
    25, MONS_TROLL, MONS_TROLL, MH_NATURAL, 60,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 15},
      {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    8, 960,
    0, 10, MST_SNORG, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SNORG}, TILE_ERROR
},

{
    MONS_PARGI, 'T', GREEN, "Pargi",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN | M_MALE,
    MR_NO_FLAGS,
    10, MONS_TROLL, MONS_TROLL, MH_NATURAL, 10,
    { {AT_BITE, AF_PLAIN, 9}, {AT_CLAW, AF_PLAIN, 4},
      {AT_CLAW, AF_PLAIN, 4}, AT_NO_ATK },
    5, 350,
    1, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PARGI}, TILE_ERROR
},

{
    MONS_PARGHIT, 'T', GREEN, "Parghit",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN | M_MALE,
    MR_NO_FLAGS,
    10, MONS_TROLL, MONS_TROLL, MH_NATURAL, 140,
    { {AT_BITE, AF_PLAIN, 50}, {AT_CLAW, AF_PLAIN, 40},
      {AT_CLAW, AF_PLAIN, 40}, AT_NO_ATK },
    27, 1200,
    1, 12, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PARGHIT}, TILE_ERROR
},

// Semi-unique in the moon base wizlab
{
    MONS_MOON_TROLL, 'T', LIGHTCYAN, "moon troll",
    M_WARM_BLOOD | M_SPEAKS | M_FAST_REGEN | M_SEE_INVIS
        | M_NO_POLY_TO | M_NO_GEN_DERIVED,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_POISON,
    10, MONS_TROLL, MONS_MOON_TROLL, MH_NATURAL, 140,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 25},
      {AT_CLAW, AF_PLAIN, 25}, AT_NO_ATK },
    18, 990,
    20, 4, MST_MOON_TROLL, false, S_SHOUT,
    I_HUMAN, HT_LAND, 12, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MOON_TROLL}, TILE_ERROR
},

// "V"ampires.
{
    MONS_JORY, 'V', LIGHTRED, "Jory",
    M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_MALE
        | M_WARM_BLOOD | M_UNIQUE,
    MR_RES_COLD,
    13, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, 160,
    { {AT_HIT, AF_PLAIN, 40}, {AT_BITE, AF_VAMPIRIC, 15}, AT_NO_ATK,
       AT_NO_ATK },
    18, 1800,
    10, 15, MST_JORY, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JORY}, TILE_ERROR
},

// Snails and other gastropods.
{
    MONS_GASTRONOK, 'w', MAGENTA, "Gastronok",
    M_UNIQUE | M_SEE_INVIS | M_SPEAKS | M_NO_WAND | M_MALE,
    MR_NO_FLAGS,
    16, MONS_ELEPHANT_SLUG, MONS_ELEPHANT_SLUG, MH_NATURAL, 80,
    { {AT_BITE, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    20, 1500,
    2, 1, MST_GASTRONOK, true, S_GURGLE,
    I_HUMAN, HT_AMPHIBIOUS, 5, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT, MON_SHAPE_SNAIL,
    {TILEP_MONS_GASTRONOK}, TILE_ERROR
},

// Horrors
{
    MONS_MLIOGLOTL, 'X', BROWN, "Mlioglotl",
    M_UNIQUE | M_SPEAKS | M_MALE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD,
    13, MONS_THRASHING_HORROR, MONS_MLIOGLOTL, MH_DEMONIC, 120,
    { {AT_HIT, AF_PLAIN, 20}, {AT_TRAMPLE, AF_TRAMPLE, 25}, AT_NO_ATK, AT_NO_ATK },
    15, 1400,
    10, 5, MST_MLIOGLOTL, false, S_SHOUT,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_MISC,
    {TILEP_MONS_MLIOGLOTL}, TILE_ERROR
},

// Elephants.
{
    MONS_NELLIE, 'Y', LIGHTMAGENTA, "Nellie",
    M_WARM_BLOOD | M_UNIQUE | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    8, MONS_ELEPHANT, MONS_HELLEPHANT, MH_DEMONIC, 140,
    { {AT_TRAMPLE, AF_TRAMPLE, 45}, {AT_BITE, AF_PLAIN, 20},
      {AT_GORE, AF_PLAIN, 15 }, AT_NO_ATK },
    20, 2400,
    13, 10, MST_HELLEPHANT, true, S_TRUMPET,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_GIANT, MON_SHAPE_QUADRUPED,
    {TILEP_MONS_NELLIE}, TILE_ERROR
},

// Skulls and "z" undead.
{
    MONS_MURRAY, 'z', LIGHTRED, "Murray",
    M_UNIQUE | M_SEE_INVIS | M_SPEAKS | M_MALE,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC,
    15, MONS_CURSE_SKULL, MONS_CURSE_SKULL, MH_UNDEAD, WILL_INVULN,
    { {AT_BITE, AF_PLAIN, 20}, {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    14, 1610,
    30, 10, MST_MURRAY, false, S_MOAN,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_TINY, MON_SHAPE_MISC,
    {TILEP_MONS_MURRAY}, TILE_ERROR
},

// numbers!
{
    MONS_IGNACIO, '1', LIGHTMAGENTA, "Ignacio",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_MALE,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD | MR_RES_POISON,
    14, MONS_EXECUTIONER, MONS_EXECUTIONER, MH_DEMONIC, 160,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 10}, {AT_HIT, AF_PLAIN, 10},
      {AT_HIT, AF_PLAIN, 5} },
    18, 2520,
    10, 15, MST_IGNACIO, false, S_SHOUT,
    I_HUMAN, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_IGNACIO}, TILE_ERROR
},

{
    MONS_GRINDER, '5', BLUE, "Grinder",
    M_UNIQUE | M_SPEAKS | M_NO_HT_WAND | M_FEMALE,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    11, MONS_SHADOW_IMP, MONS_SHADOW_IMP, MH_DEMONIC, 20,
    { {AT_HIT, AF_PAIN, 11}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 390,
    3, 11, MST_GRINDER, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LITTLE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GRINDER}, TILE_ERROR
},

{
    MONS_AMAEMON, '6', GREEN, "Amaemon",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_DEMONSPAWN, MONS_DEMONSPAWN, MH_NATURAL | MH_DEMONIC, 60,
    { {AT_HIT, AF_PLAIN, 12}, {AT_CLAW, AF_PLAIN, 12},
      {AT_TAIL_SLAP, AF_POISON, 8}, AT_NO_ATK },
    7, 875,
    3, 12, MST_AMAEMON, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_AMAEMON}, TILE_ERROR
},

{
    // Roxanne obviously can't use items, but we want to equip her with
    // a spellbook, so MONUSE_STARTING_EQUIPMENT is necessary.
    MONS_ROXANNE, '8', BLUE, "Roxanne",
    M_UNIQUE | M_STATIONARY | M_SPEAKS | M_NO_WAND | M_FEMALE,
    mrd(MR_RES_FIRE | MR_RES_COLD, 2) | MR_RES_ELEC
        | MR_RES_PETRIFY,
    10, MONS_STATUE, MONS_STATUE, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 1820,
    20, 0, MST_ROXANNE, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ROXANNE}, TILE_ERROR
},

{
    MONS_VV, '9', MAGENTA, "Vv",
    M_UNIQUE | M_FEMALE,
    MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC | MR_RES_PETRIFY,
    10, MONS_VV, MONS_VV, MH_NONLIVING, 140,
    { {AT_HIT, AF_COLD, 45}, {AT_HIT, AF_FIRE, 45}, AT_NO_ATK, AT_NO_ATK },
    23, 2700,
    27, 12, MST_VV, false, S_SILENT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_VV}, TILE_ERROR
},

// human uniques
{
    MONS_TERENCE, '@', LIGHTCYAN, "Terence",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    2, 200,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_TERENCE}, TILE_ERROR
},

{
    MONS_JESSICA, '@', LIGHTGREY, "Jessica",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    125, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 95,
    0, 10, MST_JESSICA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JESSICA}, TILE_ERROR
},

{
    MONS_SIGMUND, '@', YELLOW, "Sigmund",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    55, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 10,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    3, 270,
    0, 11, MST_ORC_WIZARD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, ATTACK_ENERGY(15),
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_SIGMUND}, TILE_ERROR
},

{
    MONS_EDMUND, '@', RED, "Edmund",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 440,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EDMUND}, TILE_ERROR
},

{
    MONS_PSYCHE, '@', LIGHTMAGENTA, "Psyche",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 400,
    0, 12, MST_PSYCHE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_PSYCHE}, TILE_ERROR
},

{
    MONS_DONALD, '@', BLUE, "Donald",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 840,
    3, 10, MST_DONALD, true, S_SHOUT,
    I_HUMAN, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DONALD}, TILE_ERROR
},

{
    MONS_JOSEPH, '@', CYAN, "Joseph",
    M_UNIQUE | M_FIGHTER | M_ARCHER | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 40,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    6, 660,
    0, 10, MST_NO_SPELLS, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JOSEPH}, TILE_ERROR
},

{
    MONS_ERICA, '@', MAGENTA, "Erica",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 630,
    0, 11, MST_ERICA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ERICA}, TILE_ERROR
},

{
    MONS_JOSEPHINE, '@', WHITE, "Josephine",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    mrd(MR_RES_NEG, 3),
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 11}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    10, 700,
    0, 10, MST_JOSEPHINE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_JOSEPHINE}, TILE_ERROR
},

{
    MONS_HAROLD, '@', LIGHTGREEN, "Harold",
    M_UNIQUE | M_FIGHTER | M_WARM_BLOOD | M_MALE | M_SPEAKS,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 60,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    9, 765,
    0, 8, MST_HAROLD, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_HAROLD}, TILE_ERROR
},

{
    MONS_LOUISE, '@', LIGHTMAGENTA, "Louise",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 80,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    13, 1040,
    0, 10, MST_LOUISE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LOUISE}, TILE_ERROR
},

{
    MONS_FRANCES, '@', YELLOW, "Frances",
    M_UNIQUE | M_WARM_BLOOD | M_SEE_INVIS | M_FEMALE | M_SPEAKS,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 29}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    14, 1330,
    0, 10, MST_FRANCES, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FRANCES}, TILE_ERROR
},

{
    MONS_RUPERT, '@', LIGHTRED, "Rupert",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    18, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1200,
    0, 10, MST_RUPERT, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_RUPERT}, TILE_ERROR
},

{
    MONS_KIRKE, '@', LIGHTBLUE, "Kirke",
    M_UNIQUE | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 100,
    { {AT_HIT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 1120,
    0, 10, MST_KIRKE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_KIRKE}, TILE_ERROR
},

{
    MONS_FREDERICK, '@', GREEN, "Frederick",
    M_UNIQUE | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    15, MONS_DEMIGOD, MONS_DEMIGOD, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    21, 1575,
    0, 10, MST_FREDERICK, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_FREDERICK}, TILE_ERROR
},

{
    MONS_MAGGIE, '@', LIGHTRED, "Maggie",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 600,
    0, 10, MST_MAGGIE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MAGGIE}, TILE_ERROR
},

{
    MONS_MARGERY, '@', LIGHTRED, "Margery",
    M_UNIQUE | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS | M_FEMALE,
    MR_NO_FLAGS,
    15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 140,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    22, 1650,
    0, 10, MST_MARGERY, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MARGERY}, TILE_ERROR
},

{
    MONS_EUSTACHIO, '@', BLUE, "Eustachio",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE,
    MR_NO_FLAGS,
    20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 20,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    4, 400,
    0, 13, MST_EUSTACHIO, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_EUSTACHIO}, TILE_ERROR
},

{
    MONS_MAURICE, '@', GREEN, "Maurice",
    M_UNIQUE | M_WARM_BLOOD | M_SPEAKS | M_MALE | M_ALWAYS_WAND,
    MR_NO_FLAGS,
    24, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 20,
    { {AT_HIT, AF_STEAL, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    5, 600,
    1, 13, MST_MAURICE, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MAURICE}, TILE_ERROR
},

{
    MONS_NIKOLA, '@', LIGHTCYAN, "Nikola",
    M_UNIQUE | M_WARM_BLOOD | M_SEE_INVIS | M_MALE | M_SPEAKS,
    MR_NO_FLAGS, // Xom would hate MR_RES_ELEC here.
    13, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, 120,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 1890,
    1, 9, MST_NIKOLA, true, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM, MON_SHAPE_HUMANOID,
    {TILEP_MONS_NIKOLA}, TILE_ERROR
},

// unique major demons ('&')
{
    MONS_MNOLEG, '&', LIGHTGREEN, "Mnoleg",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_TALL_TILE
        | M_GENDER_NEUTRAL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE,
    15, MONS_PANDEMONIUM_LORD, MONS_MNOLEG, MH_DEMONIC, WILL_INVULN,
    { {AT_CLAW, AF_PLAIN, 40}, {AT_TENTACLE_SLAP, AF_MUTATE, 35},
      {AT_GORE, AF_PLAIN, 30}, {AT_KICK, AF_BLINK, 23} },
    17, 3485,
    10, 25, MST_MNOLEG, false, S_BUZZ,
    I_HUMAN, HT_LAND, 13, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_MNOLEG}, TILE_ERROR
},

{
    MONS_LOM_LOBON, '&', LIGHTBLUE, "Lom Lobon",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_FLIES | M_TALL_TILE
        | M_GENDER_NEUTRAL,
    MR_RES_POISON | MR_RES_FIRE | mrd(MR_RES_COLD | MR_RES_ELEC, 3),
    15, MONS_PANDEMONIUM_LORD, MONS_LOM_LOBON, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_ANTIMAGIC, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    19, 3895,
    10, 20, MST_LOM_LOBON, false, S_SCREAM,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_LOM_LOBON}, TILE_ERROR
},

{
    MONS_CEREBOV, '&', RED, "Cerebov",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_TALL_TILE
        | M_GENDER_NEUTRAL,
    MR_RES_POISON | MR_RES_DAMNATION | mrd(MR_RES_FIRE, 3),
    15, MONS_PANDEMONIUM_LORD, MONS_CEREBOV, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 60}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    21, 6510,
    30, 8, MST_CEREBOV, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_GIANT, MON_SHAPE_HUMANOID,
    {TILEP_MONS_CEREBOV}, TILE_ERROR
},

{
    MONS_GLOORX_VLOQ, '&', LIGHTGREY, "Gloorx Vloq",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_FLIES | M_TALL_TILE
        | M_GENDER_NEUTRAL,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC,
    15, MONS_PANDEMONIUM_LORD, MONS_GLOORX_VLOQ, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 3520,
    10, 10, MST_GLOORX_VLOQ, false, S_MOAN,
    I_HUMAN, HT_LAND, 20, DEFAULT_ENERGY,
    MONUSE_OPEN_DOORS, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_GLOORX_VLOQ}, TILE_ERROR
},

{
    MONS_GERYON, '&', GREEN, "Geryon",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_MALE | M_FLIES
        | M_TALL_TILE,
    MR_NO_FLAGS,
    15, MONS_HELL_LORD, MONS_GERYON, MH_DEMONIC, 120,
    { {AT_TAIL_SLAP, AF_REACH, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    15, 3000,
    15, 6, MST_GERYON, false, S_ROAR,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
        MON_SHAPE_HUMANOID_WINGED,
    {TILEP_MONS_GERYON}, TILE_ERROR
},

{
    MONS_DISPATER, '&', MAGENTA, "Dispater",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_TALL_TILE
        | M_GENDER_NEUTRAL,
    mrd(MR_RES_ELEC | MR_RES_FIRE, 3) | MR_RES_POISON | MR_RES_DAMNATION
        | MR_RES_COLD,
    15, MONS_HELL_LORD, MONS_DISPATER, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    16, 4480,
    40, 3, MST_DISPATER, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_DISPATER}, TILE_ERROR
},

{
    MONS_ASMODEUS, '&', LIGHTRED, "Asmodeus",
    M_UNIQUE | M_FIGHTER | M_SEE_INVIS | M_SPEAKS | M_MALE | M_FLIES
        | M_TALL_TILE | M_FIRE_RING,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_DAMNATION | mrd(MR_RES_FIRE, 3),
    25, MONS_HELL_LORD, MONS_ASMODEUS, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    17, 4505,
    30, 7, MST_ASMODEUS, false, S_SHOUT,
    I_HUMAN, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ASMODEUS}, TILE_ERROR
},

{
    MONS_ERESHKIGAL, '&', WHITE, "Ereshkigal",
    M_UNIQUE | M_SEE_INVIS | M_SPEAKS | M_FEMALE | M_TALL_TILE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD,
    15, MONS_HELL_LORD, MONS_ERESHKIGAL, MH_DEMONIC, WILL_INVULN,
    { {AT_HIT, AF_DRAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    18, 3510,
    10, 30, MST_ERESHKIGAL, false, S_SHOUT,
    I_HUMAN, HT_LAND, 14, DEFAULT_ENERGY,
    MONUSE_WEAPONS_ARMOUR, SIZE_LARGE, MON_SHAPE_HUMANOID,
    {TILEP_MONS_ERESHKIGAL}, TILE_ERROR
},

// Impossible to hit, impossible to damage, immune to everything,
// unkillable, just sits there doing nothing but casting Shadow Creatures
// over and over.
{
    MONS_TEST_SPAWNER, 'X', WHITE, "test spawner",
    M_STATIONARY | M_INSUBSTANTIAL | M_NO_POLY_TO | M_FAST_REGEN,
    mrd(MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD
        | MR_RES_MIASMA | MR_RES_ACID, 4) | MR_RES_STICKY_FLAME,
    15, MONS_TEST_SPAWNER, MONS_TEST_SPAWNER, MH_NONLIVING, WILL_INVULN,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1000, 10000000,
    127, 127, MST_TEST_SPAWNER, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 14, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_TEST_SPAWNER}, TILE_ERROR
},

// tons of hp, just sits there doing nothing except being a target.
{
    MONS_TEST_STATUE, 'X', WHITE, "test statue",
    M_STATIONARY | M_NO_POLY_TO | M_FAST_REGEN | M_NO_THREAT,
    MR_NO_FLAGS,
    15, MONS_TEST_STATUE, MONS_TEST_STATUE, MH_NONLIVING, 20,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1000, 10000000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 14, SWIM_ENERGY(6),
    MONUSE_NOTHING, SIZE_LARGE, MON_SHAPE_MISC,
    {TILEP_MONS_TEST_SPAWNER}, TILE_ERROR
},

// tons of hp, does not attack, but not stationary
{
    MONS_TEST_BLOB, 'J', WHITE, "test blob",
    M_NO_POLY_TO | M_FAST_REGEN | M_NO_THREAT,
    MR_NO_FLAGS,
    15, MONS_TEST_STATUE, MONS_TEST_STATUE, MH_NONLIVING, 20,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1000, 10000000,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_BRAINLESS, HT_AMPHIBIOUS, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILEP_MONS_JELLY}, TILE_ERROR
},


// an unspecified monster
{
    MONS_SENSED, '{', RED, "sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_UNSEEN_MONSTER}
},

{
    MONS_SENSED_FRIENDLY, '{', GREEN, "friendly sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_MONS_SENSED_FRIENDLY}, TILE_ERROR
},

{
    MONS_SENSED_TRIVIAL, '{', BLUE, "trivial sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_MONS_SENSED_TRIVIAL}, TILE_ERROR
},

{
    MONS_SENSED_EASY, '{', LIGHTGRAY, "easy sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_MONS_SENSED_EASY}, TILE_ERROR
},

{
    MONS_SENSED_TOUGH, '{', YELLOW, "tough sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_MONS_SENSED_TOUGH}, TILE_ERROR
},

{
    MONS_SENSED_NASTY, '{', LIGHTRED, "nasty sensed monster",
    M_CANT_SPAWN,
    MR_NO_FLAGS,
    0, MONS_SENSED, MONS_SENSED, MH_NONLIVING, 0,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    1, 10,
    0, 0, MST_NO_SPELLS, false, S_SILENT,
    I_ANIMAL, HT_LAND, 10, DEFAULT_ENERGY,
    MONUSE_NOTHING, SIZE_MEDIUM, MON_SHAPE_MISC,
    {TILE_MONS_SENSED_NASTY}, TILE_ERROR
},

};
