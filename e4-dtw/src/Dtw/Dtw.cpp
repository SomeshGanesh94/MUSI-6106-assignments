
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
    if (iNumCols<=0 || iNumRows <=0)
    {
        return kFunctionInvalidArgsError;
    }
    m_ppfCostMatrix = new m_sNode* [iNumRows];
    for (int row = 0; row < iNumRows; row++)
    {
        m_ppfCostMatrix[row] = new m_sNode [iNumCols];
    }
    
    m_iNumCols = iNumCols;
    m_iNumRows = iNumRows;
    
    m_PathLength = 0;
    m_PathCost = 0.0;
    
    return kNoError;
}

Error_t CDtw::reset()
{
    for (int row = 0; row < m_iNumRows; row++)
    {
        delete m_ppfCostMatrix[row];
    }
    delete [] m_ppfCostMatrix;
    m_ppfCostMatrix = NULL;
    
    m_PathLength = 0;
    m_PathCost = 0.0;
    
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    if (ppfDistanceMatrix == 0)
    {
        return kFunctionInvalidArgsError;
    }
    if (*ppfDistanceMatrix==NULL)
    {
        return kNotInitializedError;
    }
    
    m_ppfCostMatrix[0][0].value = ppfDistanceMatrix[0][0];
    m_ppfCostMatrix[0][0].length = 1;
    
    if (m_iNumRows!=1)
    {
        for (int row = 1; row < m_iNumRows; row++)
        {
            m_ppfCostMatrix[row][0].value = m_ppfCostMatrix[row-1][0].value + ppfDistanceMatrix[row][0];
            m_ppfCostMatrix[row][0].direction = kVert;
            m_ppfCostMatrix[row][0].length = m_ppfCostMatrix[row-1][0].length + 1;
        }
    }
    
    if (m_iNumCols!=1)
    {
        for (int col = 1; col < m_iNumCols; col++)
        {
            m_ppfCostMatrix[0][col].value = m_ppfCostMatrix[0][col-1].value + ppfDistanceMatrix[0][col];
            m_ppfCostMatrix[0][col].direction = kHoriz;
            m_ppfCostMatrix[0][col].length = m_ppfCostMatrix[0][col-1].length + 1;
        }
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
                    m_ppfCostMatrix[row][col].length = m_ppfCostMatrix[row][col-1].length + 1;
                }
                else
                {
                    m_ppfCostMatrix[row][col].value = fTempValue + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = dTempDirection;
                    m_ppfCostMatrix[row][col].length = m_ppfCostMatrix[row-1][col-1].length + 1;
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
                    m_ppfCostMatrix[row][col].length = m_ppfCostMatrix[row-1][col].length + 1;
                }
                else
                {
                    m_ppfCostMatrix[row][col].value = fTempValue + ppfDistanceMatrix[row][col];
                    m_ppfCostMatrix[row][col].direction = dTempDirection;
                    m_ppfCostMatrix[row][col].length = m_ppfCostMatrix[row-1][col-1].length + 1;
                }
            }
        }
    }
    
    m_PathLength = m_ppfCostMatrix[m_iNumRows-1][m_iNumCols-1].length;
    m_PathCost = m_ppfCostMatrix[m_iNumRows-1][m_iNumCols-1].value;
    
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
    int row = m_iNumRows-1;
    int col = m_iNumCols-1;
    
    for (int ele = m_PathLength-1; ele >= 0 ; ele--)
    {
        ppiPathResult[0][ele] = row;
        ppiPathResult[1][ele] = col;
        
        switch (m_ppfCostMatrix[row][col].direction)
        {
            case kDiag:
                row -= 1;
                col -= 1;
                break;
            case kVert:
                row -= 1;
                break;
            case kHoriz:
                col -= 1;
                break;
        }
    }
    
    return kNoError;
}

