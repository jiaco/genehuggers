#ifndef	GH_MIRSCAN_H
#define	GH_MIRSCAN_H	1
namspace	GH
{
class	MirScan
{
public:
	bool	search( const QList<Row>& query, const Seq& seq,
		 QList<Hyb>& result );
private:
	Duplex		duplex;
	MirParam	param;
};
}	//	GH namespace
#endif
