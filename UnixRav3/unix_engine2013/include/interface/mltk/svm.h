#ifndef _LIBSVM_H
#define _LIBSVM_H

#define LIBSVM_VERSION 317

#pragma pack(1)

typedef LE_I32			IntL;
typedef LE_F64			IEEE754_Float64;
typedef IEEE754_Float64	Float64;

typedef struct SVMNode
{
	IntL			idx;
	Float64			valu;
} RS_PACK_ONEBYTE SVMNode;

enum SVMTYPE	{ C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
enum KERNELTYPE { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */

typedef struct SVMParameter
{
	UINT8			SvmType;		// SVMTYPE
	UINT8			KernelType;		// KERNELTYPE
	IntL			degree;			/* for poly */
	Float64			gamma;			/* for poly/rbf/sigmoid */
	Float64			coef0;			/* for poly/sigmoid */
} RS_PACK_ONEBYTE SVMParameter;

//
// SVMModel
// 

enum {
	DF_PROBA	= 1,
	DF_PROBB	= 2,
};

typedef struct SVMModelHdr
{
	SVMParameter	Param;		/* parameter */
	IntL			NumClass;	/* number of classes, = 2 in regression/one class svm */
	IntL			NumSV;		/* total #SV */
	UINT8			Flags;
	UINT8			ProgIdCHC;	// char count of progid
} RS_PACK_ONEBYTE SVMModelHdr;

enum ModelFrom
{
	FromNull,
	FromLoad,
	FromAttach,
};

typedef struct SVMModel : SVMModelHdr
{
	Float64 *	rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
	Float64 *	ProbA;		/* pariwise probability information [k*(k-1)/2] */
	Float64 *	ProbB;		/* [k*(k-1)/2] */
	IntL*		Labels;		/* label of each class (label[k]) */
	IntL*		nSVPerClass;/* number of SVs for each class (nSV[k]) */
	SVMNode*	XSpace;		/* X-Space */
	////////////////////////////////////////////////////////////////////////////////////////////////
	SVMNode**	SVs;		/* SVs (SV[l]) */
	Float64**	SVCoef;		/* coefficients for SVs in decision functions (SVCoef[nr_class-1][l]) */
	LPSTR		Adapter;	// Sample Productor
	ModelFrom	From;
} SVMModel;

#pragma pack()

bool	svm_model_load( RX_BLOB& rBlob, SVMModel& rModel);
bool	svm_model_load( LPCSTR aFileName, SVMModel& rModel);
bool	svm_model_load( LPSTR aText, SIZE_T cbText, SVMModel& rModel);
bool	svm_model_load( FILE* fp, SVMModel& rModel);
void	svm_model_clear( SVMModel& rModel );

int		svm_model_dump( const SVMModel& rModel, void * p, size_t cb );		// < 0, fail; > 0 && > cb, buf too small; < cb

double	svm_predict_values(const SVMModel* rModel, const SVMNode *x, Float64* dec_values);
double	svm_predict(const SVMModel* rModel, const SVMNode *x);
// double	svm_predict_probability(const SVMModel *model, const SVMNode *x, double* prob_estimates);
int svm_check_probability_model(const SVMModel& rModel);

#endif /* _LIBSVM_H */
