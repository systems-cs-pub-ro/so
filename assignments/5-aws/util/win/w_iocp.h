/*
 * IoCompletionPorts wrapper functions
 *
 * 2011-2017, Operating Systems
 */

#ifndef W_IOCP_H_
#define W_IOCP_H_	1

#ifdef __cplusplus
extern "C" {
#endif


static HANDLE w_iocp_create(void)
{
	return CreateIoCompletionPort(
			INVALID_HANDLE_VALUE,
			NULL,
			(ULONG_PTR) NULL,
			0);
}

static HANDLE w_iocp_add_handle(HANDLE iocp, HANDLE hFile)
{
	return CreateIoCompletionPort(
			hFile,
			iocp,
			(ULONG_PTR) hFile,
			0);
}

static HANDLE w_iocp_add_key(HANDLE iocp, HANDLE hFile, ULONG_PTR key)
{
	return CreateIoCompletionPort(
			hFile,
			iocp,
			key,
			0);
}

static BOOL w_iocp_wait(HANDLE iocp, PDWORD bytes, PULONG_PTR key,
	LPOVERLAPPED *op)
{
	return GetQueuedCompletionStatus(
			iocp,
			bytes,
			key,
			op,
			INFINITE);
}

#ifdef __cplusplus
}
#endif

#endif /* W_IOCP_H_ */
