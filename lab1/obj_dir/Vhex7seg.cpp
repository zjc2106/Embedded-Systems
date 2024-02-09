// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vhex7seg.h for the primary calling header

#include "Vhex7seg.h"
#include "Vhex7seg__Syms.h"

//==========
CData/*6:0*/ Vhex7seg::__Vtable1_y[16];

VL_CTOR_IMP(Vhex7seg) {
    Vhex7seg__Syms* __restrict vlSymsp = __VlSymsp = new Vhex7seg__Syms(this, name());
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vhex7seg::__Vconfigure(Vhex7seg__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vhex7seg::~Vhex7seg() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vhex7seg::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vhex7seg::eval\n"); );
    Vhex7seg__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("hex7seg.sv", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vhex7seg::_eval_initial_loop(Vhex7seg__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("hex7seg.sv", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vhex7seg::_combo__TOP__1(Vhex7seg__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_combo__TOP__1\n"); );
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vtableidx1 = vlTOPp->a;
    vlTOPp->y = vlTOPp->__Vtable1_y[vlTOPp->__Vtableidx1];
}

void Vhex7seg::_eval(Vhex7seg__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_eval\n"); );
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

void Vhex7seg::_eval_initial(Vhex7seg__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_eval_initial\n"); );
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vhex7seg::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::final\n"); );
    // Variables
    Vhex7seg__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vhex7seg::_eval_settle(Vhex7seg__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_eval_settle\n"); );
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

VL_INLINE_OPT QData Vhex7seg::_change_request(Vhex7seg__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_change_request\n"); );
    Vhex7seg* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vhex7seg::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((a & 0xf0U))) {
        Verilated::overWidthError("a");}
}
#endif  // VL_DEBUG

void Vhex7seg::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhex7seg::_ctor_var_reset\n"); );
    // Body
    a = VL_RAND_RESET_I(4);
    y = VL_RAND_RESET_I(7);
    __Vtableidx1 = 0;
    __Vtable1_y[0] = 0x40U;
    __Vtable1_y[1] = 0x79U;
    __Vtable1_y[2] = 0x24U;
    __Vtable1_y[3] = 0x30U;
    __Vtable1_y[4] = 0x19U;
    __Vtable1_y[5] = 0x12U;
    __Vtable1_y[6] = 2U;
    __Vtable1_y[7] = 0x78U;
    __Vtable1_y[8] = 0U;
    __Vtable1_y[9] = 0x10U;
    __Vtable1_y[10] = 8U;
    __Vtable1_y[11] = 3U;
    __Vtable1_y[12] = 0x46U;
    __Vtable1_y[13] = 0x21U;
    __Vtable1_y[14] = 6U;
    __Vtable1_y[15] = 0xeU;
}
