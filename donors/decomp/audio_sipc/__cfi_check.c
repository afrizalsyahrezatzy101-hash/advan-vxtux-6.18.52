// signature: undefined __cfi_check(void)

void __cfi_check(long param_1,undefined1 *param_2,undefined8 param_3)

{
  undefined1 *puVar1;
  
  if (param_1 < 0x7ff5a213d37b1a6) {
    if (param_1 < -0x25f41a7ab8fea45e) {
      if (param_1 < -0x631f31ee7603c3b1) {
        if (param_1 == -0x734b09deb920018a) {
          puVar1 = &LAB_00107694;
        }
        else if (param_1 == -0x6ef86220e8ff92f4) {
          puVar1 = &LAB_0010764c;
        }
        else {
          if (param_1 != -0x6b7d9f646c25cf62) goto LAB_0010550c;
          puVar1 = &LAB_00107690;
        }
      }
      else if (param_1 < -0x38162e00706dfb96) {
        if (param_1 == -0x631f31ee7603c3b1) {
          puVar1 = &LAB_00107680;
LAB_001054fc:
          if (((ulong)((long)param_2 - (long)puVar1) >> 2 | (long)param_2 << 0x3e) < 2) {
            return;
          }
          goto LAB_0010550c;
        }
        if (param_1 != -0x45ce73be2dfb584e) goto LAB_0010550c;
        puVar1 = &LAB_00107640;
      }
      else {
        if (param_1 != -0x38162e00706dfb96) {
          if (param_1 != -0x3557318fb8d07b71) goto LAB_0010550c;
          puVar1 = &LAB_00107668;
          goto LAB_001054fc;
        }
        puVar1 = &LAB_0010767c;
      }
    }
    else if (param_1 < -0x9fff7a1f5d276c3) {
      if (param_1 == -0x25f41a7ab8fea45e) {
        puVar1 = &LAB_00107650;
      }
      else if (param_1 == -0x158f0b0c250d73d0) {
        puVar1 = &LAB_00107648;
      }
      else {
        if (param_1 != -0xfaf8b6d0939286c) goto LAB_0010550c;
        puVar1 = &LAB_00107674;
      }
    }
    else if (param_1 < -0x8962c8020a87446) {
      if (param_1 != -0x9fff7a1f5d276c3) {
        if (param_1 != -0x8dc801903aca538) goto LAB_0010550c;
        puVar1 = (undefined1 *)0x1076ac;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107644;
    }
    else {
      if (param_1 != -0x8962c8020a87446) {
        if (param_1 != 0x2b3a43e29242445) goto LAB_0010550c;
        puVar1 = (undefined1 *)0x1076b8;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107678;
    }
  }
  else if (param_1 < 0x3c3507c0ff3a7aff) {
    if (param_1 < 0x28cd34a3b6815913) {
      if (param_1 == 0x7ff5a213d37b1a6) {
        puVar1 = &LAB_001076a8;
      }
      else {
        if (param_1 != 0xc185a8305be510f) {
          if (param_1 != 0x26b69e04af459c2a) goto LAB_0010550c;
          puVar1 = &LAB_00107658;
          goto LAB_001054fc;
        }
        puVar1 = &LAB_00107638;
      }
    }
    else if (param_1 < 0x34d1c2fe15049aef) {
      if (param_1 == 0x28cd34a3b6815913) {
        puVar1 = &LAB_00107634;
      }
      else {
        if (param_1 != 0x321969b888d4949e) goto LAB_0010550c;
        puVar1 = &LAB_001076a4;
      }
    }
    else {
      if (param_1 != 0x34d1c2fe15049aef) {
        if (param_1 != 0x3bf49544a6c9deff) goto LAB_0010550c;
        puVar1 = &LAB_0010769c;
        goto LAB_001054fc;
      }
      puVar1 = &LAB_00107688;
    }
  }
  else if (param_1 < 0x57641507abf06d7d) {
    if (param_1 == 0x3c3507c0ff3a7aff) {
      puVar1 = &LAB_00107698;
    }
    else if (param_1 == 0x5233d5bc7887ae44) {
      puVar1 = &LAB_001076b4;
    }
    else {
      if (param_1 != 0x55e179eb2d73f26e) goto LAB_0010550c;
      puVar1 = &LAB_0010768c;
    }
  }
  else if (param_1 < 0x75c7cc3c6e221ec8) {
    if (param_1 == 0x57641507abf06d7d) {
      puVar1 = &LAB_0010763c;
    }
    else {
      if (param_1 != 0x5e77f5d343869bc9) goto LAB_0010550c;
      puVar1 = &LAB_00107654;
    }
  }
  else {
    if (param_1 == 0x75c7cc3c6e221ec8) {
      puVar1 = &LAB_00107660;
      goto LAB_001054fc;
    }
    if (param_1 != 0x7e04a0fb7ad8bcd5) goto LAB_0010550c;
    puVar1 = (undefined1 *)0x107670;
  }
  if (param_2 == puVar1) {
    return;
  }
LAB_0010550c:
  __cfi_check_fail(param_3);
  return;
}



