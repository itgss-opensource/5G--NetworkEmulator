/**********************************************
* Copyright 2022 Nokia
* Licensed under the BSD 3-Clause Clear License
* SPDX-License-Identifier: BSD-3-Clause-Clear
**********************************************/

#pragma once

#include <math.h>

#include <mac_layer/harq_handler.h>
#include <utils/conversions.h>
#include <utils/logging/mean_handler.h>
#include <pkts/pkts.h>

//--------------------------------------------------------------------------------------------------
// ip_buffer(): this class stores the simulated (coming from purely virtual or actual IP packets)
// IP packets. It's a custom buffer implementation which include some statistics tracking, such 
// as drop, release or generation rates. 
// Input: 
//      _verbosity: Enable/disable verbosity
//--------------------------------------------------------------------------------------------------
class ip_buffer
{
public: 
    ip_buffer(int _verbosity = 0);
private: 
    std::deque<ip_pkt> pkt_list; 
    int dropped_pkts = 0; 
    float current_t; 
    int current_id = 0; 
    float oldest_t = 0; 
    int verbosity = 0; 

private: 
    int max_size = 65500*8*20; 
    float current_size = 0; 

private: 
    mean_handler<float> g_mean; 
    mean_handler<float> e_mean; 

public: 
    float get_oldest_timestamp();
    bool has_pkts();
    void generate(float bits, float pkt_size, float t, float bh_d, float bh_d_var);
    float drop_pkt(int bits);
    void step(float _current_t);
    float get_pkts(float _bits, harq_pkt& pkt);
    float get_pkts(float _bits);
    void consume_bits(float bits);
    float get_generated(bool partial = true);
    float get_error(bool partial = true);
    void add_pkt(ip_pkt pkt);
    bool buffer_full(); 
};