// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vrange__Syms.h"


//======================

void Vrange::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vrange* t = (Vrange*)userthis;
    Vrange__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
        t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void Vrange::traceChgThis(Vrange__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vrange* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY((1U & (vlTOPp->__Vm_traceActivity 
                               | (vlTOPp->__Vm_traceActivity 
                                  >> 1U))))) {
            vlTOPp->traceChgThis__2(vlSymsp, vcdp, code);
        }
        if (VL_UNLIKELY((1U & (vlTOPp->__Vm_traceActivity 
                               | (vlTOPp->__Vm_traceActivity 
                                  >> 2U))))) {
            vlTOPp->traceChgThis__3(vlSymsp, vcdp, code);
        }
        if (VL_UNLIKELY((2U & vlTOPp->__Vm_traceActivity))) {
            vlTOPp->traceChgThis__4(vlSymsp, vcdp, code);
        }
        vlTOPp->traceChgThis__5(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vrange::traceChgThis__2(Vrange__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vrange* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+1,(vlTOPp->range__DOT__running));
        vcdp->chgBit(c+9,(vlTOPp->range__DOT__we_trig));
        vcdp->chgBit(c+17,(vlTOPp->range__DOT__din_trig));
        vcdp->chgBit(c+25,(vlTOPp->range__DOT__we));
    }
}

void Vrange::traceChgThis__3(Vrange__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vrange* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBus(c+33,(vlTOPp->range__DOT__addr),4);
    }
}

void Vrange::traceChgThis__4(Vrange__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vrange* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+41,(vlTOPp->range__DOT__cgo));
        vcdp->chgBit(c+49,((1U == vlTOPp->range__DOT__c1__DOT__dout)));
        vcdp->chgBus(c+57,(vlTOPp->range__DOT__n),32);
        vcdp->chgBus(c+65,(vlTOPp->range__DOT__num),4);
        vcdp->chgBus(c+73,(vlTOPp->range__DOT__din),16);
        vcdp->chgBus(c+81,(vlTOPp->range__DOT__mem[0]),16);
        vcdp->chgBus(c+82,(vlTOPp->range__DOT__mem[1]),16);
        vcdp->chgBus(c+83,(vlTOPp->range__DOT__mem[2]),16);
        vcdp->chgBus(c+84,(vlTOPp->range__DOT__mem[3]),16);
        vcdp->chgBus(c+85,(vlTOPp->range__DOT__mem[4]),16);
        vcdp->chgBus(c+86,(vlTOPp->range__DOT__mem[5]),16);
        vcdp->chgBus(c+87,(vlTOPp->range__DOT__mem[6]),16);
        vcdp->chgBus(c+88,(vlTOPp->range__DOT__mem[7]),16);
        vcdp->chgBus(c+89,(vlTOPp->range__DOT__mem[8]),16);
        vcdp->chgBus(c+90,(vlTOPp->range__DOT__mem[9]),16);
        vcdp->chgBus(c+91,(vlTOPp->range__DOT__mem[10]),16);
        vcdp->chgBus(c+92,(vlTOPp->range__DOT__mem[11]),16);
        vcdp->chgBus(c+93,(vlTOPp->range__DOT__mem[12]),16);
        vcdp->chgBus(c+94,(vlTOPp->range__DOT__mem[13]),16);
        vcdp->chgBus(c+95,(vlTOPp->range__DOT__mem[14]),16);
        vcdp->chgBus(c+96,(vlTOPp->range__DOT__mem[15]),16);
        vcdp->chgBus(c+209,(vlTOPp->range__DOT__c1__DOT__dout),32);
    }
}

void Vrange::traceChgThis__5(Vrange__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vrange* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+217,(vlTOPp->clk));
        vcdp->chgBit(c+225,(vlTOPp->go));
        vcdp->chgBus(c+233,(vlTOPp->start),32);
        vcdp->chgBit(c+241,(vlTOPp->done));
        vcdp->chgBus(c+249,(vlTOPp->count),16);
    }
}
