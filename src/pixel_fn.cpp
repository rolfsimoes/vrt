#include <gdal.h>
#include <cpl_string.h>
#include <Rcpp.h>

CPLErr TestFunction(void **papoSources, int nSources, void *pData,
                    int nXSize, int nYSize,
                    GDALDataType eSrcType, GDALDataType eBufType,
                    int nPixelSpace, int nLineSpace)
{
  int ii, iLine, iCol;
  double pix_val;
  double x0, x3, x4, x8;

  // ---- Init ----
  if (nSources != 4) return CE_Failure;

  // ---- Set pixels ----
  for( iLine = 0; iLine < nYSize; iLine++ )
  {
    for( iCol = 0; iCol < nXSize; iCol++ )
    {
      ii = iLine * nXSize + iCol;
      /* Source raster pixels may be obtained with SRCVAL macro */
      x0 = SRCVAL(papoSources[0], eSrcType, ii);
      x3 = SRCVAL(papoSources[1], eSrcType, ii);
      x4 = SRCVAL(papoSources[2], eSrcType, ii);
      x8 = SRCVAL(papoSources[3], eSrcType, ii);

      pix_val = 0;

      GDALCopyWords(&pix_val, GDT_UInt16, 0,
                    ((GByte *)pData) + nLineSpace * iLine + iCol * nPixelSpace,
                    eBufType, nPixelSpace, 1);
    }
  }

  // ---- Return success ----
  return CE_None;
}

using namespace Rcpp;

// [[Rcpp::export]]

int register_func() {
  return GDALAddDerivedBandPixelFunc("MyFirstFunction", TestFunction);
}
