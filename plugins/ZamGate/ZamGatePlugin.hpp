/*
 * ZamGate
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#ifndef ZAMGATEPLUGIN_HPP_INCLUDED
#define ZAMGATEPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#define MAX_GATE 400

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGatePlugin : public Plugin
{
public:
    enum Parameters
    {
        paramAttack,
	paramRelease,
	paramThresh,
	paramMakeup,
	paramOutputLevel,
	paramGainR,
        paramCount
    };

    ZamGatePlugin();
    ~ZamGatePlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamGate";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'G', 'A', 'T');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const ;
    void  d_setParameterValue(uint32_t index, float value) ;
    void  d_setProgram(uint32_t index) ;

    // -------------------------------------------------------------------
    void d_activate() ;
    void d_deactivate() ;
    void d_run(const float** inputs, float** outputs, uint32_t frames) override;

	static inline float
	from_dB(float gdb) {
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10(g));
	}

    // -------------------------------------------------------------------
    float attack,release,thresdb,makeup,gainr,outlevel; //parameters

	void pushsample(float samples[100], float sample);
	float averageabs(float samples[100]);

	float samples[MAX_GATE];
	float gatestate, oldsample;
	int pos;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif
