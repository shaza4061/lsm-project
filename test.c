#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lsm.h"
#include "test.h"

// This code is designed to test the correctness of your implementation.
// You do not need to significantly change it.
// Compile and run it in the command line by typing:
// make test; ./test

int runTest(lsm* tree)
{
    int num_tests = 512;
    keyType keys[] = { 18504, 15250, 27869, 3871, 10005, 14011, 10819, 3638, 22547, 10349, 1193, 27065, 27112, 15791,
	91, 11516, 15487, 3177, 6166, 14674, 32549, 9213, 7893, 29668, 22962, 5419, 11852, 16464, 16828, 28713, 31927,
	13814, 8821, 10018, 2609, 24303, 3245, 29788, 9716, 5829, 11484, 7033, 25649, 9666, 18308, 29447, 8582, 23559,
	3639, 4684, 951, 10044, 21441, 6347, 31191, 4646, 20023, 26907, 18962, 18413, 29115, 1222, 17796, 10622, 696,
	18371, 18631, 23346, 7851, 23027, 24011, 3895, 17360, 26101, 11548, 29332, 6682, 4903, 23070, 10569, 19777,
	19450, 24813, 12770, 15267, 8094, 15124, 18642, 30673, 9759, 16953, 31457, 21582, 24278, 5206, 1310, 1993,
	11944, 5062, 18639, 28512, 2753, 2561, 11258, 17709, 28862, 14231, 5129, 16863, 4491, 13327, 21265, 25543, 7624,
	16566, 26808, 30557, 27280, 7926, 9173, 27080, 19894, 26342, 1800, 32612, 25174, 2397, 11684, 30017, 2136,
	22763, 18364, 23463, 29678, 31939, 20943, 32473, 24823, 28305, 1078, 23817, 24145, 31882, 9525, 12731, 19726,
	20847, 21693, 19723, 31668, 30913, 14772, 16454, 17056, 9484, 15933, 1780, 23657, 26941, 16468, 23578, 17519,
	16653, 20521, 15358, 3800, 6899, 11671, 8446, 5256, 23305, 16451, 19786, 5209, 23606, 26293, 29309, 27578, 2880,
	1145, 4183, 15703, 14204, 10535, 12414, 11874, 12711, 18403, 32494, 12931, 15581, 22036, 5698, 11012, 2184,
	9016, 6077, 4015, 15248, 24842, 18768, 1708, 3566, 988, 18822, 4543, 5366, 3825, 22243, 17100, 21385, 8915,
	17197, 18722, 27930, 10750, 20234, 13168, 29134, 8458, 9517, 6879, 12880, 5054, 7495, 12417, 22486, 13087,
	15358, 1226, 18715, 2580, 13449, 5811, 11829, 5704, 26282, 755, 13229, 14468, 31263, 31115, 32240, 9179, 30727,
	4568, 4708, 27144, 9266, 26319, 6222, 7737, 13410, 28805, 3623, 11304, 27115, 23020, 3706, 21430, 23500, 3096,
	30199, 16253, 3313, 11826, 24496, 485, 18814, 28450, 11760, 19867, 32316, 9023, 29044, 21610, 2138, 18458, 9235,
	23609, 6245, 4802, 27015, 7790, 18289, 18855, 30276, 23098, 5338, 21689, 9056, 27134, 12385, 4935, 1482, 32371,
	14817, 20456, 30156, 2453, 488, 3990, 22171, 25484, 30080, 16738, 4610, 22182, 2755, 27975, 15575, 16929, 6277,
	19726, 18808, 17760, 8938, 10549, 27707, 2241, 11676, 16693, 27693, 28627, 2041, 26331, 5072, 18041, 17668,
	27164, 28088, 13192, 5374, 17028, 30952, 14837, 4146, 13073, 4782, 27442, 6784, 17743, 9094, 7655, 3699, 2641,
	26189, 18011, 2242, 1063, 1931, 19297, 21579, 26998, 22533, 11114, 22068, 31315, 31073, 19715, 25656, 8528,
	6694, 16228, 8274, 23652, 14590, 13894, 11087, 26882, 27397, 31775, 21050, 15419, 14998, 21173, 19950, 8286,
	29094, 22134, 28756, 31872, 9282, 26269, 16187, 10464, 8440, 21889, 6862, 15476, 25542, 27404, 29682, 30873,
	1810, 12077, 8941, 24654, 13163, 10484, 7107, 31972, 217, 11773, 6658, 27706, 25541, 12314, 27124, 28121, 23119,
	24445, 1882, 16582, 31626, 15779, 2713, 18447, 21460, 28358, 28721, 83, 20676, 30559, 27636, 17402, 16638,
	20277, 9527, 11582, 9519, 15833, 417, 2389, 3323, 7053, 23538, 32568, 20045, 13470, 29131, 8675, 7247, 8767,
	27149, 24267, 21860, 31918, 26419, 18577, 12583, 29745, 21723, 23337, 25892, 11092, 4347, 13517, 1669, 29706,
	15903, 16757, 28964, 27252, 569, 1134, 19572, 13299, 728, 17832, 30632, 11688, 2050, 21996, 1809, 20146, 28458,
	13232, 26460, 16623, 8442, 3563, 6934, 10565, 30486, 24918, 25843, 3385, 6153, 31724, 9172, 11430, 21140, 24403,
	12756, 13347, 94, 7290, 10358, 17439, 11990, 28309, 22798, 1131, 5807, 17429, 7030, 25264, 32225, 10064, 1710,
	2055 };

    valType values[] = { 3894, 8595, 10945, 29497, 9839, 18701, 29860, 21533, 16764, 17204, 2544, 18699, 1508, 20595,
	10512, 30092, 17317, 24798, 2475, 13717, 12647, 28833, 27518, 6527, 17454, 10928, 32387, 13263, 19359, 4388,
	13068, 21521, 7994, 30615, 28410, 21550, 8664, 18766, 5158, 18919, 15535, 7625, 21473, 17387, 30446, 6078,
	22198, 20514, 30509, 10674, 25626, 17501, 17710, 24960, 30576, 27026, 26133, 26837, 26078, 27847, 24755, 26156,
	24047, 21674, 28872, 26181, 18839, 24943, 9538, 5516, 9458, 5581, 18250, 24153, 4893, 26233, 1854, 2356, 7114,
	27939, 670, 17869, 28981, 20344, 23454, 22254, 10718, 6605, 25776, 20844, 8782, 15915, 26649, 18279, 17982,
	4874, 2445, 30889, 8137, 11789, 15808, 30679, 14681, 27491, 3558, 29455, 16758, 9531, 8864, 15457, 225, 11880,
	13359, 5049, 26018, 20668, 21251, 20860, 8027, 30452, 7414, 21615, 22776, 29192, 25631, 11070, 20896, 26618,
	24785, 11793, 24131, 2697, 9569, 32579, 4538, 19086, 14909, 22513, 32571, 29332, 12196, 10372, 2926, 32475,
	27912, 11332, 440, 23908, 23567, 20559, 717, 18820, 4789, 15146, 17662, 28823, 23271, 25602, 13210, 21413,
	31334, 21041, 9146, 28479, 6338, 22808, 20572, 13756, 24350, 5340, 2140, 25037, 23922, 32055, 16527, 835, 2474,
	4983, 24086, 32279, 1668, 9114, 21581, 5965, 18729, 9383, 22594, 19240, 5074, 9861, 28054, 8024, 13016, 32579,
	3112, 31762, 25499, 22118, 24334, 1446, 23266, 15433, 27614, 7510, 26649, 11533, 6035, 14403, 11980, 14252,
	2916, 24807, 19278, 22469, 15794, 1632, 26938, 22725, 750, 28692, 14665, 17854, 16079, 27755, 32406, 11487,
	28320, 13834, 21484, 17177, 10800, 19954, 19009, 31607, 15535, 2647, 20918, 32339, 32399, 23140, 29445, 17599,
	27714, 28240, 10735, 4662, 6164, 18196, 30246, 6157, 27737, 15748, 12509, 4438, 13581, 26639, 5317, 7005, 1284,
	17885, 13575, 26694, 12384, 1207, 28849, 24685, 1809, 25786, 29838, 6136, 11747, 6223, 1507, 26477, 5314, 25152,
	18544, 16552, 27275, 11637, 18572, 26323, 16579, 19935, 31426, 19285, 16955, 19459, 28898, 5727, 11022, 4154,
	20888, 17685, 32420, 17183, 5346, 4202, 32710, 5313, 30079, 22679, 7725, 10334, 20023, 4540, 19423, 8960, 16753,
	25021, 7193, 5134, 5404, 22905, 10689, 18143, 15679, 29971, 18129, 17342, 27181, 14114, 7697, 28899, 14363, 95,
	29216, 4564, 302, 21592, 26177, 20033, 3735, 23468, 32050, 25680, 9211, 15930, 22590, 26043, 25074, 27422, 5862,
	16071, 696, 32026, 17501, 17836, 7052, 5889, 18459, 27966, 3572, 27778, 16172, 20011, 26140, 21295, 5750, 1774,
	24226, 25268, 10298, 5055, 30817, 5319, 30245, 4099, 8379, 24507, 23411, 22229, 8749, 9172, 8943, 28185, 4217,
	30856, 3208, 10411, 30696, 236, 1191, 25543, 9085, 28716, 16437, 28257, 245, 21377, 9605, 10444, 18309, 24554,
	28385, 3614, 25488, 998, 30393, 20252, 19767, 18063, 28815, 21759, 10594, 17001, 1280, 22535, 12134, 25463,
	31355, 3380, 22452, 21488, 12729, 20668, 26106, 1361, 3949, 18097, 3551, 7010, 12805, 27412, 29526, 28784,
	15812, 30228, 13870, 29939, 30329, 29362, 5351, 24379, 11371, 3491, 23492, 11626, 2909, 27829, 27850, 21597,
	8539, 20932, 31707, 32353, 3857, 13930, 24305, 8948, 8526, 1080, 29234, 19069, 27847, 27416, 29406, 13300,
	17449, 30786, 5158, 26344, 630, 956, 13475, 8952, 22067, 10332, 17251, 31922, 956, 8025, 31577, 9985, 17739,
	22606, 28590, 9307, 22636, 7725, 4784, 24206, 18762, 916, 19402, 18927, 18905, 18037, 4737, 19876, 18466, 19584,
	31807, 7678, 27672, 8257, 27627, 7132, 5433, 18624, 7634, 20202, 25606, 14895, 17999, 22599, 8612, 1008, 4498,
	8059, 21052, 23787 };

    printf("Testing putting and getting from the hash table.\n");
    printf("Inserting %d key-value pairs.\n", num_tests);
    for(int i = 0; i < num_tests; i += 1) {
	put(tree, keys[i], values[i]);
	printf("\t%d: (%d -> %d) \n", i, keys[i], values[i]);
    }

    for(int i = 0; i < num_tests; i += 1) {
	int index = rand() % num_tests;
	keyType target_key = keys[index];
	pair value = get(tree, target_key);
	if((value.state == VALID && value.value != values[index]) || value.state != VALID) {
	    printf(
	        "Test failed with key %d. Got value %d. Expected value %d.\n", target_key, value.value, values[index]);
	    return 1;
	}
	printf("Test key:%d. PASSED\n", target_key);
    }

    printf("Passed tests for putting and getting.\n");
    printf("Now testing erasing.\n");

    for(int i = 0; i < num_tests; i += 1) {
	keyType target_key = keys[i];
	erase(tree, target_key);
	pair value = get(tree, target_key);
	if(value.state == VALID) {
	    printf("Test failed with key %d. Expected it to be erased, but got %d.\n", target_key, value.value);
	    return 1;
	}
    }

    printf("Passed tests for erasing.\n");
    printf("Dumping LSM Tree content.\n");
    printTree(tree);
    free(tree);
    printf("All tests have been successfully passed.\n");
    return 0;
}

int main()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio);
    runTest(tree);
    return 0;
}
