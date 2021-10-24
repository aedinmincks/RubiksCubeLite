#include "RubiksCubeBase.h"

CRubiksCubeBase::CRubiksCubeBase(int n)
{
    size = n;
    currentState.assign(n, 0);
    goalState.assign(n, 0);
}

bool CRubiksCubeBase::SetState(vi v)
{
    if (v.size() == size)
    {
        currentState = v;
        return true;
    }

    return false;
}

void CRubiksCubeBase::DoMove(int move)
{
    currentState = ApplyMove(move, currentState);
}

vi CRubiksCubeBase::ApplyMove(int move, vi v)
{
    return vi();
}
