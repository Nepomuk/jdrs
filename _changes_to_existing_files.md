changes to existing files
=========================

sm_sregs.vhd
------------

### 1)

__find__
    or (    SL2SLV(not fi_valid and P_REG and not P_WR and B2SL(P_A = SM_RO_DATA))
       and x"EEEEEEEE")
    or (    SL2SLV(fi_valid and (   blk or DT_ACK or (P_REG and not P_WR and B2SL(P_A = SM_RO_DATA))))
       and fi_dout)

__in there replace_
    P_REG

__with__
    s_rdy
