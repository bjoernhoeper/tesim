/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <iostream>     // std::cout, std::ostream, std::ios
#include <utility>
#include <iomanip>
#include <string>

#include "TEGEErrorChannel.h"

TEGEErrorChannel::TEGEErrorChannel(pq_pair error_rate, unsigned dlen, const double* init_values, int seed)
	: m_error_rate(error_rate), m_dlen(dlen), m_previous(0), m_chan_state(0), m_distribution(0.0, 1.0), m_numberGenerator(m_generator, m_distribution)
{
	m_previous = new double[m_dlen]();
	m_chan_state = new bool[m_dlen]();
	std::fill_n(m_chan_state, dlen, true); // init to good state

	// copy the initial values into state memory
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		m_previous[ii] = init_values[ii];
	}

	// refer to http://www.radmangames.com/programming/generating-random-numbers-in-c
	// seed the generator
	m_generator.seed(seed); // seed with the current time 
}

TEGEErrorChannel::~TEGEErrorChannel()
{
	delete m_previous;
}

double TEGEErrorChannel::operator()()
{
	return m_numberGenerator();
}

double* TEGEErrorChannel::operator+(double* data)
{
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		// roll the dice to determine state fo this increment
		double rnd = (*this)();
		if (m_chan_state[ii]) // channel is up
		{
			// test for transition to channel down/error state
			m_chan_state[ii] = (rnd <= m_error_rate.first) ? false : true;
		}
		else
		{
			// test for transition to channel up/good state
			m_chan_state[ii] = (rnd <= m_error_rate.second) ? true : false;
		}

		// take action on the data based on links state
		if (!m_chan_state[ii])
		{
			// a packet error has occured.  apply previous value.
			data[ii] = m_previous[ii];
		}

		// retain the data for the next increment
		m_previous[ii] = data[ii];
	}
	return data;
}

// overloaded output stream for channel
std::ostream& operator<< (std::ostream& lhs, const TEGEErrorChannel& rhs)
{
	unsigned dlen = rhs.dlen();
	for (unsigned ii = 0; ii < dlen-1; ii++)
	{
		lhs << rhs.chan_state()[ii] << "\t";
	}
	lhs << rhs.chan_state()[dlen-1];
	return lhs;
}
