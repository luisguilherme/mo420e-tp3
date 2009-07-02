
#ifndef XPRS_MSE_DEFAULTHANDLER
#define XPRS_MSE_DEFAULTHANDLER

#include <xprs.h>

static int XPRS_CC XPRS_mse_defaulthandler(XPRSmipsolenum mse, XPRSprob prob, XPRSmipsolpool msp, void *ctx, int * nMaxSols, const double * x_Zb, const int nCols, const double dMipObject, double * dModifiedObject, int * bRejectSoln, int * bUpdateMipAbsCutOffOnCurrentSet)
{
  int nDeleted = 0;
  int bStorageIsFullOnEntry = 0;
  int *cull_sol_id_list = NULL;

  int nMaxSolsToCull_MIPOBJECT = -1;
  int nMaxSolsToCull_DIVERSITY = -1;

  if(*nMaxSols > 0) {
    int nSoln;

    if(XPRS_mse_getintattrib(mse, XPRS_MSE_SOLUTIONS, &nSoln)) {
      goto exit_with_failure;
    }

    if(nSoln >= *nMaxSols) {
      /* 
      Keeping the new solution will put the number of solutions above the max limit. 
      Either choose to ignore the new solution and/or delete some solutions already 
      stored.
      */
      int i, nSolsToCull, iSolutionIdStatus, nMaxSolsToCull = 1;

      bStorageIsFullOnEntry = 1;

      /* Get some parameters to use to run the routine calls here */
      if(XPRS_mse_getintcontrol(mse, XPRS_MSE_CALLBACKCULLSOLS_MIPOBJECT, &nMaxSolsToCull_MIPOBJECT)) {
        goto exit_with_failure;
      }
      nMaxSolsToCull = nMaxSolsToCull < nMaxSolsToCull_MIPOBJECT ? nMaxSolsToCull_MIPOBJECT : nMaxSolsToCull;
      if(XPRS_mse_getintcontrol(mse, XPRS_MSE_CALLBACKCULLSOLS_DIVERSITY, &nMaxSolsToCull_DIVERSITY)) {
        goto exit_with_failure;
      }
      nMaxSolsToCull = nMaxSolsToCull < nMaxSolsToCull_DIVERSITY ? nMaxSolsToCull_DIVERSITY : nMaxSolsToCull;

      /* Allocate some memory to store the list of existing solutions we may be culling here */
      if(!(cull_sol_id_list = (int *)malloc(nMaxSolsToCull * sizeof(int)))) {
        goto exit_with_failure;
      }

      /* Try culling some existing solutions and maybe choose to ignore the new solution as well. */
      if(nMaxSolsToCull_MIPOBJECT >= 0) {
        if(XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_MIPOBJECT, cull_sol_id_list, nMaxSolsToCull_MIPOBJECT, &nSolsToCull, dMipObject, NULL, 0, bRejectSoln)) {
          goto exit_with_failure;
        }
        for(i = 1; i <= nSolsToCull; i++) {
          if(XPRS_msp_delsol(msp, cull_sol_id_list[i - 1], &iSolutionIdStatus)) {
            goto exit_with_failure;
          }
          nDeleted++;
        }
      }
      if(nMaxSolsToCull_DIVERSITY >= 0) {
        if(XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_DIVERSITY, cull_sol_id_list, nMaxSolsToCull_DIVERSITY, &nSolsToCull, 0.0, x_Zb, nCols, bRejectSoln)) {
          goto exit_with_failure;
        }
        for(i = 1; i <= nSolsToCull; i++) {
          if(XPRS_msp_delsol(msp, cull_sol_id_list[i - 1], &iSolutionIdStatus)) {
            goto exit_with_failure;
          }
          nDeleted++;
        }
      }

      if(!*bRejectSoln && !nDeleted) {
        /* 
        None of the policies above were able to handle the current situation.
        We need to either ignore the new solution or delete an existing 
        solution to make way for the new solution. Use the mip objective to
        decide the worst solution.
        */
        nMaxSolsToCull_MIPOBJECT = 1;
        if(XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_MIPOBJECT, cull_sol_id_list, nMaxSolsToCull_MIPOBJECT, &nSolsToCull, dMipObject, NULL, 0, bRejectSoln)) {
          goto exit_with_failure;
        }
        if(*bRejectSoln || !nSolsToCull) {
          /* We still haven't rejected any solutions */
          nMaxSolsToCull_MIPOBJECT = 0;
        } else {
          /* The new solution is better than the worst stored solution */
          if(XPRS_msp_delsol(msp, cull_sol_id_list[0], &iSolutionIdStatus)) {
            goto exit_with_failure;
          }
          nDeleted++;
        }
      }
    }
  }

solution_rejected:;
  if(bStorageIsFullOnEntry) {
    if((nDeleted == 1) ^ (*bRejectSoln != 0)) {
      /* We haven't reduced the number of solutions in storage and the storage is full */
      if(nMaxSolsToCull_MIPOBJECT >= 0) {
        /* 
        The storage is full and we are using the mip objective as a metric for 
        managing the stored solutions. Update the mip cut-off to reflect the 
        worst solution we have stored (or that we will have stored once the new 
        solution is loaded when we return from here).
        */
        *bUpdateMipAbsCutOffOnCurrentSet = 1;
      }
    }
  }
  if(cull_sol_id_list)
    free(cull_sol_id_list);
  cull_sol_id_list = NULL;
  return 0;
exit_with_failure:;
  if(cull_sol_id_list)
    free(cull_sol_id_list);
  cull_sol_id_list = NULL;
  return 1;
}

#endif
