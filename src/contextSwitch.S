.global _ZN12KernelThread13contextSwitchEPNS_7ContextES1_
_ZN12KernelThread13contextSwitchEPNS_7ContextES1_:
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)

    ret