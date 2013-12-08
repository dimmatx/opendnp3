/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __LAZY_ITERATOR_H_
#define __LAZY_ITERATOR_H_

#include <functional>
#include <assert.h>

#include <openpal/BufferWrapper.h>

#include "BufferRange.h"

namespace opendnp3
{

template <class T>
class LazyIterator : private BufferRange, public std::iterator<std::output_iterator_tag, T> 
{						
	public:

	static LazyIterator End()
	{
		return LazyIterator(openpal::ReadOnlyBuffer(nullptr, 0), 0);
	}

	typedef std::function<T (openpal::ReadOnlyBuffer&)> ReadFunction;
		
	LazyIterator(const openpal::ReadOnlyBuffer& arBuffer, size_t aNumValues, const ReadFunction& aReadFunction):
		BufferRange(arBuffer, aNumValues),
		mReadFunction(aReadFunction)
	{}

	LazyIterator(const LazyIterator& rhs) : BufferRange(rhs), mReadFunction(rhs.mReadFunction)
	{}	
				
	bool operator!=(const LazyIterator&)
	{
		return mPosition < mNumValues;		
	}

	void operator ++()
	{
		assert(mPosition < mNumValues);
		++mPosition;
	}

	T operator *()
	{	
		assert(mReadFunction);
		assert(mPosition < mNumValues);
		return mReadFunction(mBuffer);
	}

	private:		 
	LazyIterator();
	LazyIterator(const openpal::ReadOnlyBuffer& arBuffer, size_t aNumValues): BufferRange(arBuffer, aNumValues)		
	{}

	ReadFunction mReadFunction;
};

}

#endif
