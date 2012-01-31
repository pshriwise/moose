from options import *

test = { INPUT : 'adapt_test.i',
         EXODIFF : ['out.e-s002'],
         GROUP : 'adaptive'}

test_time   = { INPUT : 'adapt_time_test.i',
                EXODIFF : ['out_time.e-s007'],
                GROUP : 'adaptive'}

displaced_test = {
	INPUT : 'displaced_adapt_test.i',
	EXODIFF : ['out_displaced.e-s003' , 'displaced_adapt_test_out.e-s002'],
        GROUP : 'adaptive'}

adapt_cycles_test = {
        INPUT : 'adapt_test_cycles.i',
        EXODIFF : ['out_cycles.e', 'out_cycles.e-s002'],
        GROUP : 'adaptive'}

patch_test = { INPUT : 'patch_recovery_test.i',
               EXODIFF : ['patch_out.e-s002'],
               GROUP : 'adaptive'}

