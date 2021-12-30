#include "RubiksCubeBase.h"

CRubiksCubeBase::CRubiksCubeBase(int n)
{
    currentState.assign(n, 0);
    goalState.assign(n, 0);
}

void CRubiksCubeBase::DoMove(int move)
{
    currentState = ApplyMove(move, currentState);
}

vi CRubiksCubeBase::ApplyMove(int move, vi v)
{
    return vi();
}
