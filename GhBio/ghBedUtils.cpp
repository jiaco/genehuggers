#include "ghBedUtils.h"

namespace	GH
{
WigSet	WigSet_ValFromCount( WigSet& data )
{
	WigSet	rv = data;
	Wig	w;
	int	i = 0;
	int	j = 0;
	int	cnt = 0;
	int	lo, hi;
	int	windowSpan = 100; // no idea on what a default should be

	if( data.params().contains( "WindowSpan" ) ) {
		windowSpan = data.params()[ "WindowSpan" ].toInt();
	}
	rv.sid = data.sid;
	rv.data.clear();

	lo = data.data.at( i ).beg;
	hi = lo + windowSpan;

	while( i < data.data.size() ) {
		j = i;
		cnt = 0;
		while( j < data.data.size() && data.data.at( j ).beg < hi ) {
			++cnt;
			++j;
		}
		if( cnt > 0 ) {
			w.beg = lo;
			w.val = cnt;
			rv.data << w;
		}
		// move i to correct position
		while( i < data.data.size() && data.data.at( i ).beg < hi ) {
			++i;
		}
		lo = hi;
		hi = lo + windowSpan;
		cnt = 0;
	}
	data = rv;
	return( rv );
}
WigSet	WigSet_Smooth( const WigSet& data,
	 const QMap<QString,QVariant>& params )
{
	WigSet rv;
	int	w;
	double	loBlock =0.0, hiBlock = 0.0;
	int	i = 0;
	int	cnt = 0;
	int	firstPos, lastPos;
	

	// looks like this should check for WigSmoothingWindowSize
	//	 see solexaWizard but make sure no one else calls
	//	 this function...

	if( !params.contains( "w" ) ) {
		return( rv );
	}
	rv = data;
	rv.data.clear();
	w = params[ "w" ].toInt();
	firstPos = ( w / 2 );
	lastPos = data.data.at( data.data.size() -1 ).beg - ( w / 2 );

	for( i = 0; data.data.at( i ).beg < (w/2); ++i ) {
		loBlock += data.data.at( i ).val;
		++cnt;
	}
	if( cnt > 0 ) {
		loBlock /= cnt;
	}
	for( int pos = firstPos; pos <= lastPos; ++pos ) {
		cnt = 0;
		hiBlock = 0.0;
		while( data.data.at( i ).beg < pos + firstPos ) {
			hiBlock += data.data.at( i ).val;
			++cnt;
			++i;
		}
		if( cnt > 0 ) {
			hiBlock /= cnt;
		}
		
		Wig w;
		w.beg = pos;
		w.val = ( loBlock + hiBlock ) / 2;
		rv.data << w;

		loBlock = hiBlock;
	}
	return( rv );
}

WigList	Bounds( const WigSetList& input, const BedSetList& features,
	 const int& dws, const int& sws )
{
	WigList	rv;
	Wig	t;
	int	rp1, rp2, aux;
	int	dead = 10 * dws - 1;

	for( int idx = 0; idx < input.size(); ++idx ) {
		int	fidx = UINT;
		for( int f = 0; f < features.size(); ++f ) {
			if( features.at( f ).sid == input.at( idx ).sid ) {
				fidx = f;
				break;
			}
		}
		if( fidx == UINT ) {
			continue;
		}
		for( int i = 0; i < input.at( idx ).data.size(); ++i ) {
			for( int f = 0; f < features.at( fidx ).data.size(); ++f ) {
				rp1 = rp2 = dead;
				aux = features.at( fidx ).data.at( f ).aux;
				if( aux == 0 ) {
					rp1 = input.at( idx ).data.at( i ).beg -
						features.at( fidx ).data.at( f ).beg;
				} else if( aux == 1 ) {
					rp1 = features.at( fidx ).data.at( f ).end -
						input.at( idx ).data.at( i ).beg;
				} else {
					rp1 = input.at( idx ).data.at( i ).beg -
						features.at( fidx ).data.at( f ).beg;
					rp2 = features.at( fidx ).data.at( f ).end -
						input.at( idx ).data.at( i ).beg;
				}
				if( rp1 != dead && qAbs( rp1 ) <= dws + sws ) {
					t.beg = rp1;
					t.val = input.at( idx ).data.at( i ).val;
					rv << t;
				}
				if( rp2 != dead && qAbs( rp2 ) <= dws + sws ) {
					t.beg = rp2;
					t.val = input.at( idx ).data.at( i ).val;
					rv << t;
				}
			}
		}
	}
	qSort( rv.begin(), rv.end(), WigSort );
	return( rv );
}
WigList	Smooth( const WigList& data,
	 const int& dataSize, const int& windowSize,
	 const int& minPosition, const int& incPosition )
{
	int	lastIdx = 0;
	WigList	rv;

	for( int i = 0; i < dataSize; ++i ) {
		int lo = minPosition + ( i * incPosition );
		int hi = lo + windowSize;
		Wig t;
		t.beg = i;
		t.val = 0;
		int	cnt = 0;
		for( int j = lastIdx; j < data.size(); ++j ) {
			if( data.at( j ).beg < lo ) {
				lastIdx = j;
				continue;
			}
			if( data.at( j ).beg > hi ) {
				break;
			}
			t.val += data.at( j ).val;
			++cnt;
		}
		if( cnt > 0 ) {
			t.val /= cnt;
		}
		rv << t;
	}
	return( rv );
}

/*
 *	DOMAIN FUNCTIONS
 */
double	WigList_average( const WigList& data,
	 const double& minValue, const double& maxValue )
{
	int	cnt = 0;
	double	val = 0;
	for( int i = 0; i < data.size(); ++i ) {
		if( minValue != UDBL && data.at( i ).val < minValue ) {
			continue;
		}
		if( maxValue != UDBL && data.at( i ).val > maxValue ) {
			continue;
		}
		val += data.at( i ).val;
		++cnt;
	}
	if( cnt > 0 ) {
		val /= data.size();
	}
	return( val );
}
void	WigList_findSeeds( const WigList& data, WZoneList& seeds,
	 const int& minNumProbes, const double& minValue,
	 const double& maxValue, const double& minScore )
{
	int	cnt;
	double	val;
	int	dataEnd = data.size() - minNumProbes;
	WZone	wzone;
	wzone.aux = 0;

	for( int i = 0; i < dataEnd; ++i ) {
		if( data.at( i ).val < minValue ) {
			continue;
		}
		cnt = 0;
		val = 0;
		int j = i;
		for( ; j < i + minNumProbes; ++j ) {
			if( data.at( j ).val < minValue ) {
				continue;
			}
			val += qMin( maxValue, data.at( j ).val );
			++cnt;
		}
		if( cnt == 0 ) {
			continue;
		}
		if( ( val /= cnt ) < minScore ) {
			continue;
		}
		wzone.b = i;
		wzone.e = j;
		wzone.beg = data.at( i ).beg;
		wzone.end = data.at( j ).beg;
		wzone.val = val;

		seeds << wzone;
		i = j;
	}
	return;
}
void	WZoneList_calcScore( const WigList& data, WZoneList& wzlist,
	 const double& minValue, const double& maxValue )
{
	for( int i = 0; i < wzlist.size(); ++i ) {
		int cnt = 0;
		wzlist[ i ].val = 0;
		for( int j = wzlist[ i ].b; j < wzlist[ i ].e; ++j ) {
			if( minValue != UDBL && data.at( j ).val < minValue ) {
				continue;
			}
			if( maxValue != UDBL && data.at( j ).val > maxValue ) {
				continue;
			}
			wzlist[ i ].val += data.at( j ).val;
			++cnt;
		}
		if( cnt > 0 ) {
			wzlist[ i ].val /= cnt;
		}
	}
}
bool	WZoneSort( const WZone& e1, const WZone& e2 )
{
	if( e1.beg != e2.beg ) {
		return( e1.beg < e2.beg );
	} else {
		return( e1.end > e2.end );
	}
}
double	WZone_calcCoverage( const WZone& wz )
{
	double rv = 0;
	if( wz.end > wz.beg ) {
		rv = ( wz.e - wz.b );
		rv /= ( wz.end - wz.beg );
qDebug() << "Coverage: " << rv;
	}
	return( rv );
}
void	WZoneList_coverageFilter( WZoneList& wzlist, const double& min )
{
	for( int i = 0; i < wzlist.size(); ) {
		if( WZone_calcCoverage( wzlist.at( i ) ) < min ) {
			wzlist.removeAt( i );
		} else {
			++i;
		}
	}
}
void	WZoneList_scoreFilter( WZoneList& wzlist, const double& min )
{
	for( int i = 0; i < wzlist.size(); ) {
		if( wzlist.at( i ).val < min ) {
			wzlist.removeAt( i );
		} else {
			++i;
		}
	}
}
void	WZoneList_minSizeFilter( WZoneList& wzlist, const int& min )
{
	for( int i = 0; i < wzlist.size(); ) {
		if( ( wzlist.at( i ).end - wzlist.at( i ).beg ) < min ) {
			wzlist.removeAt( i );
		} else {
			++i;
		}
	}
}
void	WZoneList_merge( WZoneList& wzlist, const int& distance )
{
	qSort( wzlist.begin(), wzlist.end(), WZoneSort );
	for( int i = 1; i < wzlist.size(); ) {
		if( wzlist[ i ].beg < ( wzlist[ i - 1 ].end + distance ) ) {
			wzlist[ i - 1 ].e = wzlist[ i ].e;
			wzlist[ i - 1 ].end = wzlist[ i ].end;
			wzlist[ i - 1 ].val = UDBL;
			wzlist.removeAt( i );
		} else {
			++i;
		}
	}
}
int	BestHiExtend( const WigList& data, const double& maxValue,
	 const int& width, const int& pos, double& score )
{
	int	SIZE = 5;
	int	w[ SIZE ];
	int	c[ SIZE ];
	int	s[ SIZE ];

	int	best = 0;
	double	bestScore = 0.0;

	for( int i = 0; i < SIZE; ++i ) {
		w[ i ] = width * ( i + 1 );
	}
	for( int i = 0; i < SIZE; ++i ) {
		c[ i ] = 0;
		s[ i ] = 0.0;
		for( int j = 0; j < w[ i ]; ++j ) {
			if( pos + j >= data.size() ) {
				continue;
			}
			s[ i ] += qMin( maxValue, data.at( pos + j ).val );
			++c[ i ];
		}
		if( c[ i ] <= 0 ) {
			continue;
		}
		s[ i ] /= c[ i ];
		if( s[ i ] >= bestScore ) {
			bestScore = s[ i ];
			best = i;
		}
	}
	score = s[ best ];
	return( c[ best ] );
}
int	BestLoExtend( const WigList& data, const double& maxValue,
	 const int& width, const int& pos, double& score )
{
	int	SIZE = 5;
	int	w[ SIZE ];
	int	c[ SIZE ];
	int	s[ SIZE ];

	int	best = 0;
	double	bestScore = 0.0;

	for( int i = 0; i < SIZE; ++i ) {
		w[ i ] = width * ( i + 1 );
	}
	for( int i = 0; i < SIZE; ++i ) {
		c[ i ] = 0;
		s[ i ] = 0.0;
		for( int j = 0; j < w[ i ]; ++j ) {
			if( pos - j < 0 ) {
				continue;
			}
			s[ i ] += qMin( maxValue, data.at( pos - j ).val );
			++c[ i ];
		}
		if( c[ i ] <= 0 ) {
			continue;
		}
		s[ i ] /= c[ i ];
		if( s[ i ] >= bestScore ) {
			bestScore = s[ i ];
			best = i;
		}
	}
	score = s[ best ];
	return( c[ best ] );
}
int	WZoneList_extend( const WigList& data, WZoneList& wzlist,
	 const double& maxValue, const int& width, const double& pct )
{
	int	rv = 0;
	
	for( int i = 0; i < wzlist.size(); ++i ) {
		bool	extended = false;
		double	cutoff = wzlist.at( i ).val * pct;
		double	nscore = cutoff;
		int	ncnt = 0;
		while( nscore >= cutoff ) {
			ncnt = BestHiExtend( data, maxValue, width,
				wzlist[ i ].e, nscore );
			if( nscore >= cutoff ) {
				wzlist[ i ].e += ncnt;
				if( wzlist.at( i ).e > data.size() - 1 ) {
					wzlist[ i ].e = data.size() - 1;
					nscore = 0;
				}
				wzlist[ i ].end = data[ wzlist[i].e ].beg;
				extended = true;
			}
		}
		nscore = cutoff;
		while( nscore >= cutoff ) {
			ncnt = BestLoExtend( data, maxValue, width,
				 wzlist[ i ].b, nscore );
			if( nscore >= cutoff ) {
				wzlist[ i ].b -= ncnt;
				if( wzlist.at( i ).b < 0 ) {
					wzlist[ i ].b = 0;
					nscore = 0;
				}
				wzlist[ i ].beg = data[ wzlist[i].b ].beg;
				extended = true;
			}
		}
		if( extended ) {
			++rv;
		}
	}
	return( rv );
}


/*
 *	PEAK FUNCTIONS
 */
void    FormOverZones( const WigList& data, WigList& tab,
         const int& windowSize, const double& valueCutoff,
         const double& percentOver )
{
        Wig      t;

        int     b = 0;
        int     e = b + windowSize;
        double  val = 0;
        for( int i = b; i < data.size() && i < e; ++i ) {
                if( data[ i ].val > valueCutoff ) {
                        ++val;
                }
        }
        if( ( val / windowSize ) > percentOver ) {
                t.val = val;
                t.beg = b;
                tab << t;
        }
        while( e < data.size() - 1 ) {
                if( data[ b ].val > valueCutoff ) {
                        --val;
                }
                ++b;
                ++e;
                if( data[ e ].val > valueCutoff ) {
                        ++val;
                }
                if( ( val / windowSize ) > percentOver ) {
                        t.val = val;
                        t.beg = b;
                        tab << t;
                }
        }
        return;
}
void    MergeZones( const WigList& data, const WigList& dtab,
         WZoneList& mtab, const int& windowSize,
         const int& mergeDistance )
{
        WZone    t;
	t.val = 0;

        if( dtab.size() == 0 ) {
                return;
        }
        t.b = dtab[ 0 ].beg;
        t.e = t.b + windowSize;
        t.beg = data[ t.b ].beg;
        t.end = data[ t.e ].beg;
	t.aux = 0;

        for( int i = 1; i < dtab.size(); ++i ) {
                int pos = data[ dtab[ i ].beg ].beg;
                if( pos < t.end + mergeDistance ) {
                        t.e = dtab[ i ].beg + windowSize;
                        t.end = data[ t.e ].beg;
                        continue;
                }
                mtab << t;
                t.b = dtab[ i ].beg;
                t.e = t.b + windowSize;
                t.beg = data[ t.b ].beg;
                t.end = data[ t.e ].beg;
        }
        mtab << t;
}
}	//	GH namespace
