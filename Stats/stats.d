#!/usr/bin/env rdmd

// ----------------------------------------------------------------------------

interface Stat {
	void accumulate(double x);
	void postprocess();
	double result();
}

// ----------------------------------------------------------------------------

class IncrementalStat : Stat {
	protected double _result;
	abstract void accumulate(double x);
	void postprocess() {}
	double result() { return _result; }
}

// ----------------------------------------------------------------------------

import std.exception, std.stdio;

void main(string[] args) 
{
	Stat[] stats;
	foreach (arg; args[1 .. $]) {
		auto newStat = cast(Stat) Object.factory("stats." ~ arg);
		enforce(newStat, "Invalid statistics function: " ~ arg);
		stats ~= newStat;
	}
	for (double x; readf(" %s ", &x) == 1; ) {
		foreach(s; stats) {
			s.accumulate(x);
		}
	}
	foreach (s; stats) {
		s.postprocess();
		writeln(s.result());
	}
}

// ----------------------------------------------------------------------------

class Min : IncrementalStat 
{
	this() { _result = double.max; }
	override void accumulate(double x) 
	{
		if (x < _result) {
			_result = x;
		}
	}
}

// ----------------------------------------------------------------------------

class Max : IncrementalStat 
{
	this() { _result = -double.max; }
	override void accumulate(double x) 
	{
		if (x > _result) {
			_result = x;
		}
	}
}

// ----------------------------------------------------------------------------

class Avg : IncrementalStat
{
	private uint items = 0;
	this() { _result = 0; }
	override void accumulate(double x)
	{
		_result += x;
		++items;
	}
	override void postprocess() 
	{
		if (items) {
			_result /= items;
		}
	}
}

// ----------------------------------------------------------------------------
