
#include "Vector.h"
#include "Util.h"

#include "Dtw.h"

CDtw::CDtw( void )
{
    
}

CDtw::~CDtw( void )
{

}

Error_t CDtw::init( int iNumRows, int iNumCols )
{
    m_ppfCostMatrix = new m_sNode* [iNumRows];
    for (int row = 0; row < iNumRows; row++)
    {
        m_ppfCostMatrix[row] = new m_sNode [iNumCols];
    }
    
    m_iNumCols = iNumCols;
    m_iNumRows = iNumRows;
    
    return kNoError;
}

Error_t CDtw::reset()
{
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    m_ppfCostMatrix[0][0].value = ppfDistanceMatrix[0][0];
    m_ppfCostMatrix[0][0].direction = kNumDirections;
    
    for (int row = 1; row < m_iNumRows; row++)
    {
        m_ppfCostMatrix[row][0].value = m_ppfCostMatrix[row-1][0].value + ppfDistanceMatrix[row][0];
        m_ppfCostMatrix[row][0].direction = kVert;
    }
    
    for (int col = 1; col < m_iNumCols; col++)
    {
        m_ppfCostMatrix[0][col].value = m_ppfCostMatrix[col-1][0].value + ppfDistanceMatrix[0][col];
        m_ppfCostMatrix[0][col].direction = kHoriz;
    }
    
    float fTempValue;
    float fCandidateV;
    float fCandidateH;
    float fCandidateD;
    Directions_t dTempDirection;
    for (int row = 1; row < m_iNumRows; row++)
    {
        for (int col = 1; col < m_iNumCols; col++)
        {
            fCandidateD = m_ppfCostMatrix[row-1][col-1].value;
            fCandidateH = m_ppfCostMatrix[row][col-1].value;
            fCandidateV = m_ppfCostMatrix[row-1][col].value;
            
            if (fCandidateD<fCandidateV)
            {
                fTempValue = fCandidateD;
                dTempDirection = kDiag;
                if (fCandidateH < fTempValue)
                {
                    m_ppfCostMatrix[row][col].value = fCandidateH + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = kHoriz;
                }
                else
                {
                    m_ppfCostMatrix[row][col].value = fTempValue + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = dTempDirection;
                }
            }
            else
            {
                fTempValue = fCandidateV;
                dTempDirection = kVert;
                if (fCandidateH < fTempValue)
                {
                    m_ppfCostMatrix[row][col].value = fCandidateH + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = kHoriz;
                }
                else
                {
                    m_ppfCostMatrix[row][col].value = fTempValue + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = dTempDirection;
                }
            }
        }
    }
    
    return kNoError;
}

int CDtw::getPathLength()
{    
    return m_PathLength;
}

float CDtw::getPathCost() const
{
    return m_PathCost;
}

Error_t CDtw::getPath( int **ppiPathResult ) const
{
    return kNoError;
}

