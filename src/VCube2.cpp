#include "VCube2.h"

std::vector<std::string> VCube2::goal = {"UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR"};

VCube2::VCube2() : CRubiksCubeBase(16)
{
    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

VCube2::VCube2(std::vector<std::string> cubies) : CRubiksCubeBase(16)
{
    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;

        std::string cubie = cubies[i];

        while ((currentState[i] = std::find(goal.begin(), goal.end(), cubie) - goal.begin()) == 8)
        {
            cubie = cubie.substr(1) + cubie[0];
            currentState[i + 8]++;
        }
    }
}

void VCube2::Reset()
{
    size = 16;
    currentState.assign(16, 0);
    goalState.assign(16, 0);

    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

void VCube2::Show()
{
    const static std::vector<int> faces = {3,  9,  12, 1,  13, 16, 0,  17, 4,  2,  5,  8,
                                           21, 14, 11, 20, 10, 7,  22, 6,  19, 23, 18, 15};

    std::vector<char> v(24, ' ');
    int n = 0;

    for (int i = 0; i < 8; i++)
    {
        int index = currentState[i];
        int dir = currentState[i + 8];

        std::string cubie = goal[index];
        while (dir--)
        {
            cubie = cubie.substr(1) + cubie[0];
        }

        for (int j = 0; j < cubie.size(); j++)
        {
            v[faces[n++]] = cubie[j];
        }
    }

    printf("  %c%c\r\n", v[0], v[1]);
    printf("  %c%c\r\n", v[2], v[3]);
    printf("%c%c%c%c%c%c%c%c\r\n", v[4], v[5], v[8], v[9], v[12], v[13], v[16], v[17]);
    printf("%c%c%c%c%c%c%c%c\r\n", v[6], v[7], v[10], v[11], v[14], v[15], v[18], v[19]);
    printf("  %c%c\r\n", v[20], v[21]);
    printf("  %c%c\r\n", v[22], v[23]);
}

vi VCube2::ApplyMove(int move, vi state)
{
    std::vector<std::vector<int>> affectedCubiesLocation = {
        {0, 1, 2, 3}, // U
        {4, 5, 6, 7}, // D
        {0, 3, 5, 4}, // F
        {2, 1, 7, 6}, // B
        {3, 2, 6, 5}, // L
        {1, 0, 4, 7}, // R
    };

    std::vector<std::vector<int>> affectedCubiesorientation = {
        {0, 0, 0, 0}, // U
        {0, 0, 0, 0}, // D
        {2, 1, 2, 1}, // F
        {2, 1, 2, 1}, // B
        {2, 1, 2, 1}, // L
        {2, 1, 2, 1}, // R
    };

    int turns = move % 3 + 1;
    int face = move / 3;

    while (turns--)
    {
        vi oldstate = state;
        for (int i = 0; i < 4; i++)
        {
            int target = affectedCubiesLocation[face][i];
            int killer = affectedCubiesLocation[face][(i == 3) ? 0 : i + 1];
            int orientationDelta = affectedCubiesorientation[face][i];

            state[target] = oldstate[killer];
            state[target + 8] = oldstate[killer + 8] + orientationDelta;
            if (!turns)
            {
                state[target + 8] %= 3;
            }
        }
    }

    return state;
}
