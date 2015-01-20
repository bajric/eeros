#ifndef MPIPC_H
#define MPIPC_H

#include <sys/ioctl.h>


#ifdef __cplusplus
extern "C" {
#endif


#define RRPS_IOC_MAGIC  'r'
#define RRPS_REG_SIZE (32)

#define RRPS_REG  _IOW(  RRPS_IOC_MAGIC,  1, char[RRPS_REG_SIZE] )
#define RRPS_UREG _IO(   RRPS_IOC_MAGIC,  2 )
#define RRPS_ACPT _IOW(  RRPS_IOC_MAGIC,  3, int )
#define RRPS_REF  _IOW(  RRPS_IOC_MAGIC,  4, int )
#define RRPS_RECV _IOR(  RRPS_IOC_MAGIC,  5, struct mpipc_recv )
#define RRPS_REQ  _IOWR( RRPS_IOC_MAGIC,  6, struct mpipc_req )
#define RRPS_REP  _IOW(  RRPS_IOC_MAGIC,  7, struct mpipc_msg )

#define RRPS_IOC_MAXNR (7)


enum mpipc_msg_type {
	RRPS_MSG,
	RRPS_OPEN,
	RRPS_CLOSE
};


struct mpipc_msg {
	int size;
	void *buffer;
};

struct mpipc_recv {
	int type;
	int coid;
	struct mpipc_msg req;
};

struct mpipc_req {
	struct mpipc_msg req;
	struct mpipc_msg rep;
};

struct mpipc_rep {
	int coid;
	struct mpipc_msg rep;
};


#ifdef __cplusplus
}
#endif

#endif // MPIPC_H
