
/**
 * This is the default implementation of XPRSmipSolEnumHandlerListener, included in <tt>xprs.jar</tt>
 * as com.dashoptimization.XPRSdefaultMipSolEnumHandler.  The source code is also included,
 * in the file <tt>xpressmp/includes/XPRSdefaultMipSolEnumHandler.java</tt>.
 * <p />
 * The XPRSdefaultMipSolEnumHandler class implements most of the functionality you will want to
 * use with the MIP solution enumerator, but you can provide alternate implementations, possibly
 * based on XPRSdefaultMipSolEnumHandler, if you require additional functionality.
 **/

public class XPRSdefaultMipSolEnumHandler implements XPRSmipSolEnumHandlerListener {
	public XPRSdefaultMipSolEnumHandler() {
		super();
	}

  public int XPRSmipSolEnumHandlerEvent(XPRSmipsolenum mse, XPRSprob prob, XPRSmipsolpool msp, Object vContext, IntHolder nMaxSols, double[] x_Zb, int nCols, double dMipObject, DoubleHolder dModifiedObject, BoolHolder bRejectSoln, BoolHolder bUpdateMipAbsCutOffOnCurrentSet) {
		int nDeleted = 0;
		boolean bStorageIsFullOnEntry = false;
		int nMaxSolsToCull_MIPOBJECT = -1;
		int nMaxSolsToCull_DIVERSITY = -1;

		if (nMaxSols.value>0 && mse.attributes().getSolutions() >= nMaxSols.value) {
      /*
      Keeping the new solution will put the number of solutions above the max limit.
      Either choose to ignore the new solution and/or delete some solutions already
      stored.
      */
      int nMaxSolsToCull = 1;

      bStorageIsFullOnEntry = true;

      nMaxSolsToCull_MIPOBJECT = mse.controls().getCallbackCullSols_MipObject();
      nMaxSolsToCull_DIVERSITY = mse.controls().getCallbackCullSols_Diversity();


      nMaxSolsToCull = Math.max(nMaxSolsToCull, nMaxSolsToCull_MIPOBJECT);
      nMaxSolsToCull = Math.max(nMaxSolsToCull, nMaxSolsToCull_DIVERSITY);

			/* Allocate an array for storing the existing solutions we may be culling */
			int[] cull_sol_id_list = new int[nMaxSolsToCull];

			/* Try culling some existing solutions and maybe choose to ignore the new solution as well */
			if (nMaxSolsToCull_MIPOBJECT>=0) {
				int nSolsToCull = mse.getCullChoice(XPRSconstants.MSE_METRIC_MIPOBJECT, cull_sol_id_list, nMaxSolsToCull_MIPOBJECT, dMipObject, bRejectSoln);
				for (int i=1;i<=nSolsToCull;i++) {
					msp.delSol(cull_sol_id_list[i-1],null);
					nDeleted++;
				}
			}
			if (nMaxSolsToCull_DIVERSITY>=0) {
				int nSolsToCull = mse.getCullChoice(XPRSconstants.MSE_METRIC_DIVERSITY, cull_sol_id_list, nMaxSolsToCull_DIVERSITY, 0.0, x_Zb, nCols, bRejectSoln);
				for (int i=1;i<=nSolsToCull;i++) {
					msp.delSol(cull_sol_id_list[i-1],null);
					nDeleted++;
				}
			}

			if (!bRejectSoln.value && nDeleted==0) {
        /*
        None of the policies above were able to handle the current situation.
        We need to either ignore the new solution or delete an existing
        solution to make way for the new solution. Use the mip objective to
        decide the worst solution.
        */
        nMaxSolsToCull_MIPOBJECT = 1;
        int nSolsToCull = mse.getCullChoice(XPRSconstants.MSE_METRIC_MIPOBJECT, cull_sol_id_list, nMaxSolsToCull_MIPOBJECT, dMipObject, bRejectSoln);
        if (bRejectSoln.value || nSolsToCull==0) {
					/* We still haven't rejected any solutions */
					nMaxSolsToCull_MIPOBJECT = 0;
				}
				else {
					/* The new solution is better than the worst stored solution */
					msp.delSol(cull_sol_id_list[0], null);
					nDeleted++;
				}
			}
		}

		/* solution rejected */
		if (bStorageIsFullOnEntry) {
			if ((nDeleted==1) ^ bRejectSoln.value) {
				/* We haven't reduced the number of solutions in storage and the storage is full */
				if (nMaxSolsToCull_MIPOBJECT >= 0) {
					/*
					The storage is full and we are using the mip objective as a metric for
					managing the stored solutions. Update the mip cut-off to reflect the
					worst solution we have stored (or that we will have stored once the new
					solution is loaded when we return from here).
					*/
					bUpdateMipAbsCutOffOnCurrentSet.value = true;
				}
			}
		}

		return 0;
	}


}
