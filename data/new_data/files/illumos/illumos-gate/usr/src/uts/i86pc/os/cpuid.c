/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2011, 2016 by Delphix. All rights reserved.
 * Copyright 2013 Nexenta Systems, Inc. All rights reserved.
 * Copyright 2014 Josef "Jeff" Sipek <jeffpc@josefsipek.net>
 */
/*
 * Copyright (c) 2010, Intel Corporation.
 * All rights reserved.
 */
/*
 * Portions Copyright 2009 Advanced Micro Devices, Inc.
 */
/*
 * Copyright 2019 Joyent, Inc.
 */

/*
 * CPU Identification logic
 *
 * The purpose of this file and its companion, cpuid_subr.c, is to help deal
 * with the identification of CPUs, their features, and their topologies. More
 * specifically, this file helps drive the following:
 *
 * 1. Enumeration of features of the processor which are used by the kernel to
 *    determine what features to enable or disable. These may be instruction set
 *    enhancements or features that we use.
 *
 * 2. Enumeration of instruction set architecture (ISA) additions that userland
 *    will be told about through the auxiliary vector.
 *
 * 3. Understanding the physical topology of the CPU such as the number of
 *    caches, how many cores it has, whether or not it supports symmetric
 *    multi-processing (SMT), etc.
 *
 * ------------------------
 * CPUID History and Basics
 * ------------------------
 *
 * The cpuid instruction was added by Intel roughly around the time that the
 * original Pentium was introduced. The purpose of cpuid was to tell in a
 * programmatic fashion information about the CPU that previously was guessed
 * at. For example, an important part of cpuid is that we can know what
 * extensions to the ISA exist. If you use an invalid opcode you would get a
 * #UD, so this method allows a program (whether a user program or the kernel)
 * to determine what exists without crashing or getting a SIGILL. Of course,
 * this was also during the era of the clones and the AMD Am5x86. The vendor
 * name shows up first in cpuid for a reason.
 *
 * cpuid information is broken down into ranges called a 'leaf'. Each leaf puts
 * unique values into the registers %eax, %ebx, %ecx, and %edx and each leaf has
 * its own meaning. The different leaves are broken down into different regions:
 *
 *	[ 0, 7fffffff ]			This region is called the 'basic'
 *					region. This region is generally defined
 *					by Intel, though some of the original
 *					portions have different meanings based
 *					on the manufacturer. These days, Intel
 *					adds most new features to this region.
 *					AMD adds non-Intel compatible
 *					information in the third, extended
 *					region. Intel uses this for everything
 *					including ISA extensions, CPU
 *					features, cache information, topology,
 *					and more.
 *
 *					There is a hole carved out of this
 *					region which is reserved for
 *					hypervisors.
 *
 *	[ 40000000, 4fffffff ]		This region, which is found in the
 *					middle of the previous region, is
 *					explicitly promised to never be used by
 *					CPUs. Instead, it is used by hypervisors
 *					to communicate information about
 *					themselves to the operating system. The
 *					values and details are unique for each
 *					hypervisor.
 *
 *	[ 80000000, ffffffff ]		This region is called the 'extended'
 *					region. Some of the low leaves mirror
 *					parts of the basic leaves. This region
 *					has generally been used by AMD for
 *					various extensions. For example, AMD-
 *					specific information about caches,
 *					features, and topology are found in this
 *					region.
 *
 * To specify a range, you place the desired leaf into %eax, zero %ebx, %ecx,
 * and %edx, and then issue the cpuid instruction. At the first leaf in each of
 * the ranges, one of the primary things returned is the maximum valid leaf in
 * that range. This allows for discovery of what range of CPUID is valid.
 *
 * The CPUs have potentially surprising behavior when using an invalid leaf or
 * unimplemented leaf. If the requested leaf is within the valid basic or
 * extended range, but is unimplemented, then %eax, %ebx, %ecx, and %edx will be
 * set to zero. However, if you specify a leaf that is outside of a valid range,
 * then instead it will be filled with the last valid _basic_ leaf. For example,
 * if the maximum basic value is on leaf 0x3, then issuing a cpuid for leaf 4 or
 * an invalid extended leaf will return the information for leaf 3.
 *
 * Some leaves are broken down into sub-leaves. This means that the value
 * depends on both the leaf asked for in %eax and a secondary register. For
 * example, Intel uses the value in %ecx on leaf 7 to indicate a sub-leaf to get
 * additional information. Or when getting topology information in leaf 0xb, the
 * initial value in %ecx changes which level of the topology that you are
 * getting information about.
 *
 * cpuid values are always kept to 32 bits regardless of whether or not the
 * program is in 64-bit mode. When executing in 64-bit mode, the upper
 * 32 bits of the register are always set to zero so that way the values are the
 * same regardless of execution mode.
 *
 * ----------------------
 * Identifying Processors
 * ----------------------
 *
 * We can identify a processor in two steps. The first step looks at cpuid leaf
 * 0. Leaf 0 contains the processor's vendor information. This is done by
 * putting a 12 character string in %ebx, %ecx, and %edx. On AMD, it is
 * 'AuthenticAMD' and on Intel it is 'GenuineIntel'.
 *
 * From there, a processor is identified by a combination of three different
 * values:
 *
 *  1. Family
 *  2. Model
 *  3. Stepping
 *
 * Each vendor uses the family and model to uniquely identify a processor. The
 * way that family and model are changed depends on the vendor. For example,
 * Intel has been using family 0x6 for almost all of their processor since the
 * Pentium Pro/Pentium II era, often called the P6. The model is used to
 * identify the exact processor. Different models are often used for the client
 * (consumer) and server parts. Even though each processor often has major
 * architectural differences, they still are considered the same family by
 * Intel.
 *
 * On the other hand, each major AMD architecture generally has its own family.
 * For example, the K8 is family 0x10, Bulldozer 0x15, and Zen 0x17. Within it
 * the model number is used to help identify specific processors.
 *
 * The stepping is used to refer to a revision of a specific microprocessor. The
 * term comes from equipment used to produce masks that are used to create
 * integrated circuits.
 *
 * The information is present in leaf 1, %eax. In technical documentation you
 * will see the terms extended model and extended family. The original family,
 * model, and stepping fields were each 4 bits wide. If the values in either
 * are 0xf, then one is to consult the extended model and extended family, which
 * take previously reserved bits and allow for a larger number of models and add
 * 0xf to them.
 *
 * When we process this information, we store the full family, model, and
 * stepping in the struct cpuid_info members cpi_family, cpi_model, and
 * cpi_step, respectively. Whenever you are performing comparisons with the
 * family, model, and stepping, you should use these members and not the raw
 * values from cpuid. If you must use the raw values from cpuid directly, you
 * must make sure that you add the extended model and family to the base model
 * and family.
 *
 * In general, we do not use information about the family, model, and stepping
 * to determine whether or not a feature is present; that is generally driven by
 * specific leaves. However, when something we care about on the processor is
 * not considered 'architectural' meaning that it is specific to a set of
 * processors and not promised in the architecture model to be consistent from
 * generation to generation, then we will fall back on this information. The
 * most common cases where this comes up is when we have to workaround errata in
 * the processor, are dealing with processor-specific features such as CPU
 * performance counters, or we want to provide additional information for things
 * such as fault management.
 *
 * While processors also do have a brand string, which is the name that people
 * are familiar with when buying the processor, they are not meant for
 * programmatic consumption. That is what the family, model, and stepping are
 * for.
 *
 * ------------
 * CPUID Passes
 * ------------
 *
 * As part of performing feature detection, we break this into several different
 * passes. The passes are as follows:
 *
 *	Pass 0		This is a primordial pass done in locore.s to deal with
 *			Cyrix CPUs that don't support cpuid. The reality is that
 *			we likely don't run on them any more, but there is still
 *			logic for handling them.
 *
 *	Pass 1		This is the primary pass and is responsible for doing a
 *			large number of different things:
 *
 *			1. Determine which vendor manufactured the CPU and
 *			determining the family, model, and stepping information.
 *
 *			2. Gathering a large number of feature flags to
 *			determine which features the CPU support and which
 *			indicate things that we need to do other work in the OS
 *			to enable. Features detected this way are added to the
 *			x86_featureset which can be queried to
 *			determine what we should do. This includes processing
 *			all of the basic and extended CPU features that we care
 *			about.
 *
 *			3. Determining the CPU's topology. This includes
 *			information about how many cores and threads are present
 *			in the package. It also is responsible for figuring out
 *			which logical CPUs are potentially part of the same core
 *			and what other resources they might share. For more
 *			information see the 'Topology' section.
 *
 *			4. Determining the set of CPU security-specific features
 *			that we need to worry about and determine the
 *			appropriate set of workarounds.
 *
 *			Pass 1 on the boot CPU occurs before KMDB is started.
 *
 *	Pass 2		The second pass is done after startup(). Here, we check
 *			other miscellaneous features. Most of this is gathering
 *			additional basic and extended features that we'll use in
 *			later passes or for debugging support.
 *
 *	Pass 3		The third pass occurs after the kernel memory allocator
 *			has been fully initialized. This gathers information
 *			where we might need dynamic memory available for our
 *			uses. This includes several varying width leaves that
 *			have cache information and the processor's brand string.
 *
 *	Pass 4		The fourth and final normal pass is performed after the
 *			kernel has brought most everything online. This is
 *			invoked from post_startup(). In this pass, we go through
 *			the set of features that we have enabled and turn that
 *			into the hardware auxiliary vector features that
 *			userland receives. This is used by userland, primarily
 *			by the run-time link-editor (RTLD), though userland
 *			software could also refer to it directly.
 *
 *	Microcode	After a microcode update, we do a selective rescan of
 *			the cpuid leaves to determine what features have
 *			changed. Microcode updates can provide more details
 *			about security related features to deal with issues like
 *			Spectre and L1TF. On occasion, vendors have violated
 *			their contract and removed bits. However, we don't try
 *			to detect that because that puts us in a situation that
 *			we really can't deal with. As such, the only thing we
 *			rescan are security related features today. See
 *			cpuid_pass_ucode().
 *
 * All of the passes (except pass 0) are run on all CPUs. However, for the most
 * part we only care about what the boot CPU says about this information and use
 * the other CPUs as a rough guide to sanity check that we have the same feature
 * set.
 *
 * We do not support running multiple logical CPUs with disjoint, let alone
 * different, feature sets.
 *
 * ------------------
 * Processor Topology
 * ------------------
 *
 * One of the important things that we need to do is to understand the topology
 * of the underlying processor. When we say topology in this case, we're trying
 * to understand the relationship between the logical CPUs that the operating
 * system sees and the underlying physical layout. Different logical CPUs may
 * share different resources which can have important consequences for the
 * performance of the system. For example, they may share caches, execution
 * units, and more.
 *
 * The topology of the processor changes from generation to generation and
 * vendor to vendor.  Along with that, different vendors use different
 * terminology, and the operating system itself uses occasionally overlapping
 * terminology. It's important to understand what this topology looks like so
 * one can understand the different things that we try to calculate and
 * determine.
 *
 * To get started, let's talk about a little bit of terminology that we've used
 * so far, is used throughout this file, and is fairly generic across multiple
 * vendors:
 *
 * CPU
 *	A central processing unit (CPU) refers to a logical and/or virtual
 *	entity that the operating system can execute instructions on. The
 *	underlying resources for this CPU may be shared between multiple
 *	entities; however, to the operating system it is a discrete unit.
 *
 * PROCESSOR and PACKAGE
 *
 *	Generally, when we use the term 'processor' on its own, we are referring
 *	to the physical entity that one buys and plugs into a board. However,
 *	because processor has been overloaded and one might see it used to mean
 *	multiple different levels, we will instead use the term 'package' for
 *	the rest of this file. The term package comes from the electrical
 *	engineering side and refers to the physical entity that encloses the
 *	electronics inside. Strictly speaking the package can contain more than
 *	just the CPU, for example, on many processors it may also have what's
 *	called an 'integrated graphical processing unit (GPU)'. Because the
 *	package can encapsulate multiple units, it is the largest physical unit
 *	that we refer to.
 *
 * SOCKET
 *
 *	A socket refers to unit on a system board (generally the motherboard)
 *	that can receive a package. A single package, or processor, is plugged
 *	into a single socket. A system may have multiple sockets. Often times,
 *	the term socket is used interchangeably with package and refers to the
 *	electrical component that has plugged in, and not the receptacle itself.
 *
 * CORE
 *
 *	A core refers to the physical instantiation of a CPU, generally, with a
 *	full set of hardware resources available to it. A package may contain
 *	multiple cores inside of it or it may just have a single one. A
 *	processor with more than one core is often referred to as 'multi-core'.
 *	In illumos, we will use the feature X86FSET_CMP to refer to a system
 *	that has 'multi-core' processors.
 *
 *	A core may expose a single logical CPU to the operating system, or it
 *	may expose multiple CPUs, which we call threads, defined below.
 *
 *	Some resources may still be shared by cores in the same package. For
 *	example, many processors will share the level 3 cache between cores.
 *	Some AMD generations share hardware resources between cores. For more
 *	information on that see the section 'AMD Topology'.
 *
 * THREAD and STRAND
 *
 *	In this file, generally a thread refers to a hardware resources and not
 *	the operating system's logical abstraction. A thread is always exposed
 *	as an independent logical CPU to the operating system. A thread belongs
 *	to a specific core. A core may have more than one thread. When that is
 *	the case, the threads that are part of the same core are often referred
 *	to as 'siblings'.
 *
 *	When multiple threads exist, this is generally referred to as
 *	simultaneous multi-threading (SMT). When Intel introduced this in their
 *	processors they called it hyper-threading (HT). When multiple threads
 *	are active in a core, they split the resources of the core. For example,
 *	two threads may share the same set of hardware execution units.
 *
 *	The operating system often uses the term 'strand' to refer to a thread.
 *	This helps disambiguate it from the software concept.
 *
 * CHIP
 *
 *	Unfortunately, the term 'chip' is dramatically overloaded. At its most
 *	base meaning, it is used to refer to a single integrated circuit, which
 *	may or may not be the only thing in the package. In illumos, when you
 *	see the term 'chip' it is almost always referring to the same thing as
 *	the 'package'. However, many vendors may use chip to refer to one of
 *	many integrated circuits that have been placed in the package. As an
 *	example, see the subsequent definition.
 *
 *	To try and keep things consistent, we will only use chip when referring
 *	to the entire integrated circuit package, with the exception of the
 *	definition of multi-chip module (because it is in the name) and use the
 *	term 'die' when we want the more general, potential sub-component
 *	definition.
 *
 * DIE
 *
 *	A die refers to an integrated circuit. Inside of the package there may
 *	be a single die or multiple dies. This is sometimes called a 'chip' in
 *	vendor's parlance, but in this file, we use the term die to refer to a
 *	subcomponent.
 *
 * MULTI-CHIP MODULE
 *
 *	A multi-chip module (MCM) refers to putting multiple distinct chips that
 *	are connected together in the same package. When a multi-chip design is
 *	used, generally each chip is manufactured independently and then joined
 *	together in the package. For example, on AMD's Zen microarchitecture
 *	(family 0x17), the package contains several dies (the second meaning of
 *	chip from above) that are connected together.
 *
 * CACHE
 *
 *	A cache is a part of the processor that maintains copies of recently
 *	accessed memory. Caches are split into levels and then into types.
 *	Commonly there are one to three levels, called level one, two, and
 *	three. The lower the level, the smaller it is, the closer it is to the
 *	execution units of the CPU, and the faster it is to access. The layout
 *	and design of the cache come in many different flavors, consult other
 *	resources for a discussion of those.
 *
 *	Caches are generally split into two types, the instruction and data
 *	cache. The caches contain what their names suggest, the instruction
 *	cache has executable program text, while the data cache has all other
 *	memory that the processor accesses. As of this writing, data is kept
 *	coherent between all of the caches on x86, so if one modifies program
 *	text before it is executed, that will be in the data cache, and the
 *	instruction cache will be synchronized with that change when the
 *	processor actually executes those instructions. This coherency also
 *	covers the fact that data could show up in multiple caches.
 *
 *	Generally, the lowest level caches are specific to a core. However, the
 *	last layer cache is shared between some number of cores. The number of
 *	CPUs sharing this last level cache is important. This has implications
 *	for the choices that the scheduler makes, as accessing memory that might
 *	be in a remote cache after thread migration can be quite expensive.
 *
 *	Sometimes, the word cache is abbreviated with a '$', because in US
 *	English the word cache is pronounced the same as cash. So L1D$ refers to
 *	the L1 data cache, and L2$ would be the L2 cache. This will not be used
 *	in the rest of this theory statement for clarity.
 *
 * MEMORY CONTROLLER
 *
 *	The memory controller is a component that provides access to DRAM. Each
 *	memory controller can access a set number of DRAM channels. Each channel
 *	can have a number of DIMMs (sticks of memory) associated with it. A
 *	given package may have more than one memory controller. The association
 *	of the memory controller to a group of cores is important as it is
 *	cheaper to access memory on the controller that you are associated with.
 *
 * NUMA
 *
 *	NUMA or non-uniform memory access, describes a way that systems are
 *	built. On x86, any processor core can address all of the memory in the
 *	system. However, When using multiple sockets or possibly within a
 *	multi-chip module, some of that memory is physically closer and some of
 *	it is further. Memory that is further away is more expensive to access.
 *	Consider the following image of multiple sockets with memory:
 *
 *	+--------+                                                +--------+
 *	| DIMM A |         +----------+      +----------+         | DIMM D |
 *	+--------+-+       |          |      |          |       +-+------+-+
 *	  | DIMM B |=======| Socket 0 |======| Socket 1 |=======| DIMM E |
 *	  +--------+-+     |          |      |          |     +-+------+-+
 *	    | DIMM C |     +----------+      +----------+     | DIMM F |
 *	    +--------+                                        +--------+
 *
 *	In this example, Socket 0 is closer to DIMMs A-C while Socket 1 is
 *	closer to DIMMs D-F. This means that it is cheaper for socket 0 to
 *	access DIMMs A-C and more expensive to access D-F as it has to go
 *	through Socket 1 to get there. The inverse is true for Socket 1. DIMMs
 *	D-F are cheaper than A-C. While the socket form is the most common, when
 *	using multi-chip modules, this can also sometimes occur. For another
 *	example of this that's more involved, see the AMD topology section.
 *
 *
 * Intel Topology
 * --------------
 *
 * Most Intel processors since Nehalem, (as of this writing the current gen
 * is Skylake / Cannon Lake) follow a fairly similar pattern. The CPU portion of
 * the package is a single monolithic die. MCMs currently aren't used. Most
 * parts have three levels of caches, with the L3 cache being shared between
 * all of the cores on the package. The L1/L2 cache is generally specific to
 * an individual core. The following image shows at a simplified level what
 * this looks like. The memory controller is commonly part of something called
 * the 'Uncore', that used to be separate physical chips that were not a part of
 * the package, but are now part of the same chip.
 *
 *  +-----------------------------------------------------------------------+
 *  | Package                                                               |
 *  |  +-------------------+  +-------------------+  +-------------------+  |
 *  |  | Core              |  | Core              |  | Core              |  |
 *  |  |  +--------+ +---+ |  |  +--------+ +---+ |  |  +--------+ +---+ |  |
 *  |  |  | Thread | | L | |  |  | Thread | | L | |  |  | Thread | | L | |  |
 *  |  |  +--------+ | 1 | |  |  +--------+ | 1 | |  |  +--------+ | 1 | |  |
 *  |  |  +--------+ |   | |  |  +--------+ |   | |  |  +--------+ |   | |  |
 *  |  |  | Thread | |   | |  |  | Thread | |   | |  |  | Thread | |   | |  |
 *  |  |  +--------+ +---+ |  |  +--------+ +---+ |  |  +--------+ +---+ |  |
 *  |  |  +--------------+ |  |  +--------------+ |  |  +--------------+ |  |
 *  |  |  | L2 Cache     | |  |  | L2 Cache     | |  |  | L2 Cache     | |  |
 *  |  |  +--------------+ |  |  +--------------+ |  |  +--------------+ |  |
 *  |  +-------------------+  +-------------------+  +-------------------+  |
 *  | +-------------------------------------------------------------------+ |
 *  | |                         Shared L3 Cache                           | |
 *  | +-------------------------------------------------------------------+ |
 *  | +-------------------------------------------------------------------+ |
 *  | |                        Memory Controller                          | |
 *  | +-------------------------------------------------------------------+ |
 *  +-----------------------------------------------------------------------+
 *
 * A side effect of this current architecture is that what we care about from a
 * scheduling and topology perspective, is simplified. In general we care about
 * understanding which logical CPUs are part of the same core and socket.
 *
 * To determine the relationship between threads and cores, Intel initially used
 * the identifier in the advanced programmable interrupt controller (APIC). They
 * also added cpuid leaf 4 to give additional information about the number of
 * threads and CPUs in the processor. With the addition of x2apic (which
 * increased the number of addressable logical CPUs from 8-bits to 32-bits), an
 * additional cpuid topology leaf 0xB was added.
 *
 * AMD Topology
 * ------------
 *
 * When discussing AMD topology, we want to break this into three distinct
 * generations of topology. There's the basic topology that has been used in
 * family 0xf+ (Opteron, Athlon64), there's the topology that was introduced
 * with family 0x15 (Bulldozer), and there's the topology that was introduced
 * with family 0x17 (Zen). AMD also has some additional terminology that's worth
 * talking about.
 *
 * Until the introduction of family 0x17 (Zen), AMD did not implement something
 * that they considered SMT. Whether or not the AMD processors have SMT
 * influences many things including scheduling and reliability, availability,
 * and serviceability (RAS) features.
 *
 * NODE
 *
 *	AMD uses the term node to refer to a die that contains a number of cores
 *	and I/O resources. Depending on the processor family and model, more
 *	than one node can be present in the package. When there is more than one
 *	node this indicates a multi-chip module. Usually each node has its own
 *	access to memory and I/O devices. This is important and generally
 *	different from the corresponding Intel Nehalem-Skylake+ processors. As a
 *	result, we track this relationship in the operating system.
 *
 *	In processors with an L3 cache, the L3 cache is generally shared across
 *	the entire node, though the way this is carved up varies from generation
 *	to generation.
 *
 * BULLDOZER
 *
 *	Starting with the Bulldozer family (0x15) and continuing until the
 *	introduction of the Zen microarchitecture, AMD introduced the idea of a
 *	compute unit. In a compute unit, two traditional cores share a number of
 *	hardware resources. Critically, they share the FPU, L1 instruction
 *	cache, and the L2 cache. Several compute units were then combined inside
 *	of a single node.  Because the integer execution units, L1 data cache,
 *	and some other resources were not shared between the cores, AMD never
 *	considered this to be SMT.
 *
 * ZEN
 *
 *	The Zen family (0x17) uses a multi-chip module (MCM) design, the module
 *	is called Zeppelin. These modules are similar to the idea of nodes used
 *	previously. Each of these nodes has two DRAM channels which all of the
 *	cores in the node can access uniformly. These nodes are linked together
 *	in the package, creating a NUMA environment.
 *
 *	The Zeppelin die itself contains two different 'core complexes'. Each
 *	core complex consists of four cores which each have two threads, for a
 *	total of 8 logical CPUs per complex. Unlike other generations,
 *	where all the logical CPUs in a given node share the L3 cache, here each
 *	core complex has its own shared L3 cache.
 *
 *	A further thing that we need to consider is that in some configurations,
 *	particularly with the Threadripper line of processors, not every die
 *	actually has its memory controllers wired up to actual memory channels.
 *	This means that some cores have memory attached to them and others
 *	don't.
 *
 *	To put Zen in perspective, consider the following images:
 *
 *      +--------------------------------------------------------+
 *      | Core Complex                                           |
 *      | +-------------------+    +-------------------+  +---+  |
 *      | | Core       +----+ |    | Core       +----+ |  |   |  |
 *      | | +--------+ | L2 | |    | +--------+ | L2 | |  |   |  |
 *      | | | Thread | +----+ |    | | Thread | +----+ |  |   |  |
 *      | | +--------+-+ +--+ |    | +--------+-+ +--+ |  | L |  |
 *      | |   | Thread | |L1| |    |   | Thread | |L1| |  | 3 |  |
 *      | |   +--------+ +--+ |    |   +--------+ +--+ |  |   |  |
 *      | +-------------------+    +-------------------+  | C |  |
 *      | +-------------------+    +-------------------+  | a |  |
 *      | | Core       +----+ |    | Core       +----+ |  | c |  |
 *      | | +--------+ | L2 | |    | +--------+ | L2 | |  | h |  |
 *      | | | Thread | +----+ |    | | Thread | +----+ |  | e |  |
 *      | | +--------+-+ +--+ |    | +--------+-+ +--+ |  |   |  |
 *      | |   | Thread | |L1| |    |   | Thread | |L1| |  |   |  |
 *      | |   +--------+ +--+ |    |   +--------+ +--+ |  |   |  |
 *      | +-------------------+    +-------------------+  +---+  |
 *      |                                                        |
 *	+--------------------------------------------------------+
 *
 *  This first image represents a single Zen core complex that consists of four
 *  cores.
 *
 *
 *	+--------------------------------------------------------+
 *	| Zeppelin Die                                           |
 *	|  +--------------------------------------------------+  |
 *	|  |         I/O Units (PCIe, SATA, USB, etc.)        |  |
 *	|  +--------------------------------------------------+  |
 *      |                           HH                           |
 *	|          +-----------+    HH    +-----------+          |
 *	|          |           |    HH    |           |          |
 *	|          |    Core   |==========|    Core   |          |
 *	|          |  Complex  |==========|  Complex  |          |
 *	|          |           |    HH    |           |          |
 *	|          +-----------+    HH    +-----------+          |
 *      |                           HH                           |
 *	|  +--------------------------------------------------+  |
 *	|  |                Memory Controller                 |  |
 *	|  +--------------------------------------------------+  |
 *      |                                                        |
 *	+--------------------------------------------------------+
 *
 *  This image represents a single Zeppelin Die. Note how both cores are
 *  connected to the same memory controller and I/O units. While each core
 *  complex has its own L3 cache as seen in the first image, they both have
 *  uniform access to memory.
 *
 *
 *                      PP                     PP
 *                      PP                     PP
 *           +----------PP---------------------PP---------+
 *           |          PP                     PP         |
 *           |    +-----------+          +-----------+    |
 *           |    |           |          |           |    |
 *       MMMMMMMMM|  Zeppelin |==========|  Zeppelin |MMMMMMMMM
 *       MMMMMMMMM|    Die    |==========|    Die    |MMMMMMMMM
 *           |    |           |          |           |    |
 *           |    +-----------+ooo    ...+-----------+    |
 *           |          HH      ooo  ...       HH         |
 *           |          HH        oo..         HH         |
 *           |          HH        ..oo         HH         |
 *           |          HH      ...  ooo       HH         |
 *           |    +-----------+...    ooo+-----------+    |
 *           |    |           |          |           |    |
 *       MMMMMMMMM|  Zeppelin |==========|  Zeppelin |MMMMMMMMM
 *       MMMMMMMMM|    Die    |==========|    Die    |MMMMMMMMM
 *           |    |           |          |           |    |
 *           |    +-----------+          +-----------+    |
 *           |          PP                     PP         |
 *           +----------PP---------------------PP---------+
 *                      PP                     PP
 *                      PP                     PP
 *
 *  This image represents a single Zen package. In this example, it has four
 *  Zeppelin dies, though some configurations only have a single one. In this
 *  example, each die is directly connected to the next. Also, each die is
 *  represented as being connected to memory by the 'M' character and connected
 *  to PCIe devices and other I/O, by the 'P' character. Because each Zeppelin
 *  die is made up of two core complexes, we have multiple different NUMA
 *  domains that we care about for these systems.
 *
 * CPUID LEAVES
 *
 * There are a few different CPUID leaves that we can use to try and understand
 * the actual state of the world. As part of the introduction of family 0xf, AMD
 * added CPUID leaf 0x80000008. This leaf tells us the number of logical
 * processors that are in the system. Because families before Zen didn't have
 * SMT, this was always the number of cores that were in the system. However, it
 * should always be thought of as the number of logical threads to be consistent
 * between generations. In addition we also get the size of the APIC ID that is
 * used to represent the number of logical processors. This is important for
 * deriving topology information.
 *
 * In the Bulldozer family, AMD added leaf 0x8000001E. The information varies a
 * bit between Bulldozer and later families, but it is quite useful in
 * determining the topology information. Because this information has changed
 * across family generations, it's worth calling out what these mean
 * explicitly. The registers have the following meanings:
 *
 *	%eax	The APIC ID. The entire register is defined to have a 32-bit
 *		APIC ID, even though on systems without x2apic support, it will
 *		be limited to 8 bits.
 *
 *	%ebx	On Bulldozer-era systems this contains information about the
 *		number of cores that are in a compute unit (cores that share
 *		resources). It also contains a per-package compute unit ID that
 *		identifies which compute unit the logical CPU is a part of.
 *
 *		On Zen-era systems this instead contains the number of threads
 *		per core and the ID of the core that the logical CPU is a part
 *		of. Note, this ID is unique only to the package, it is not
 *		globally unique across the entire system.
 *
 *	%ecx	This contains the number of nodes that exist in the package. It
 *		also contains an ID that identifies which node the logical CPU
 *		is a part of.
 *
 * Finally, we also use cpuid leaf 0x8000001D to determine information about the
 * cache layout to determine which logical CPUs are sharing which caches.
 *
 * illumos Topology
 * ----------------
 *
 * Based on the above we synthesize the information into several different
 * variables that we store in the 'struct cpuid_info'. We'll go into the details
 * of what each member is supposed to represent and their uniqueness. In
 * general, there are two levels of uniqueness that we care about. We care about
 * an ID that is globally unique. That means that it will be unique across all
 * entities in the system. For example, the default logical CPU ID is globally
 * unique. On the other hand, there is some information that we only care about
 * being unique within the context of a single package / socket. Here are the
 * variables that we keep track of and their meaning.
 *
 * Several of the values that are asking for an identifier, with the exception
 * of cpi_apicid, are allowed to be synthetic.
 *
 *
 * cpi_apicid
 *
 *	This is the value of the CPU's APIC id. This should be the full 32-bit
 *	ID if the CPU is using the x2apic. Otherwise, it should be the 8-bit
 *	APIC ID. This value is globally unique between all logical CPUs across
 *	all packages. This is usually required by the APIC.
 *
 * cpi_chipid
 *
 *	This value indicates the ID of the package that the logical CPU is a
 *	part of. This value is allowed to be synthetic. It is usually derived by
 *	taking the CPU's APIC ID and determining how many bits are used to
 *	represent CPU cores in the package. All logical CPUs that are part of
 *	the same package must have the same value.
 *
 * cpi_coreid
 *
 *	This represents the ID of a CPU core. Two logical CPUs should only have
 *	the same cpi_coreid value if they are part of the same core. These
 *	values may be synthetic. On systems that support SMT, this value is
 *	usually derived from the APIC ID, otherwise it is often synthetic and
 *	just set to the value of the cpu_id in the cpu_t.
 *
 * cpi_pkgcoreid
 *
 *	This is similar to the cpi_coreid in that logical CPUs that are part of
 *	the same core should have the same ID. The main difference is that these
 *	values are only required to be unique to a given socket.
 *
 * cpi_clogid
 *
 *	This represents the logical ID of a logical CPU. This value should be
 *	unique within a given socket for each logical CPU. This is allowed to be
 *	synthetic, though it is usually based off of the CPU's apic ID. The
 *	broader system expects that logical CPUs that have are part of the same
 *	core have contiguous numbers. For example, if there were two threads per
 *	core, then the core IDs divided by two should be the same and the first
 *	modulus two should be zero and the second one. For example, IDs 4 and 5
 *	indicate two logical CPUs that are part of the same core. But IDs 5 and
 *	6 represent two logical CPUs that are part of different cores.
 *
 *	While it is common for the cpi_coreid and the cpi_clogid to be derived
 *	from the same source, strictly speaking, they don't have to be and the
 *	two values should be considered logically independent. One should not
 *	try to compare a logical CPU's cpi_coreid and cpi_clogid to determine
 *	some kind of relationship. While this is tempting, we've seen cases on
 *	AMD family 0xf where the system's cpu id is not related to its APIC ID.
 *
 * cpi_ncpu_per_chip
 *
 *	This value indicates the total number of logical CPUs that exist in the
 *	physical package. Critically, this is not the number of logical CPUs
 *	that exist for just the single core.
 *
 *	This value should be the same for all logical CPUs in the same package.
 *
 * cpi_ncore_per_chip
 *
 *	This value indicates the total number of physical CPU cores that exist
 *	in the package. The system compares this value with cpi_ncpu_per_chip to
 *	determine if simultaneous multi-threading (SMT) is enabled. When
 *	cpi_ncpu_per_chip equals cpi_ncore_per_chip, then there is no SMT and
 *	the X86FSET_HTT feature is not set. If this value is greater than one,
 *	than we consider the processor to have the feature X86FSET_CMP, to
 *	indicate that there is support for more than one core.
 *
 *	This value should be the same for all logical CPUs in the same package.
 *
 * cpi_procnodes_per_pkg
 *
 *	This value indicates the number of 'nodes' that exist in the package.
 *	When processors are actually a multi-chip module, this represents the
 *	number of such modules that exist in the package. Currently, on Intel
 *	based systems this member is always set to 1.
 *
 *	This value should be the same for all logical CPUs in the same package.
 *
 * cpi_procnodeid
 *
 *	This value indicates the ID of the node that the logical CPU is a part
 *	of. All logical CPUs that are in the same node must have the same value
 *	here. This value must be unique across all of the packages in the
 *	system.  On Intel based systems, this is currently set to the value in
 *	cpi_chipid because there is only one node.
 *
 * cpi_cores_per_compunit
 *
 *	This value indicates the number of cores that are part of a compute
 *	unit. See the AMD topology section for this. This member only has real
 *	meaning currently for AMD Bulldozer family processors. For all other
 *	processors, this should currently be set to 1.
 *
 * cpi_compunitid
 *
 *	This indicates the compute unit that the logical CPU belongs to. For
 *	processors without AMD Bulldozer-style compute units this should be set
 *	to the value of cpi_coreid.
 *
 * cpi_ncpu_shr_last_cache
 *
 *	This indicates the number of logical CPUs that are sharing the same last
 *	level cache. This value should be the same for all CPUs that are sharing
 *	that cache. The last cache refers to the cache that is closest to memory
 *	and furthest away from the CPU.
 *
 * cpi_last_lvl_cacheid
 *
 *	This indicates the ID of the last cache that the logical CPU uses. This
 *	cache is often shared between multiple logical CPUs and is the cache
 *	that is closest to memory and furthest away from the CPU. This value
 *	should be the same for a group of logical CPUs only if they actually
 *	share the same last level cache. IDs should not overlap between
 *	packages.
 *
 * cpi_ncore_bits
 *
 *	This indicates the number of bits that are required to represent all of
 *	the cores in the system. As cores are derived based on their APIC IDs,
 *	we aren't guaranteed a run of APIC IDs starting from zero. It's OK for
 *	this value to be larger than the actual number of IDs that are present
 *	in the system. This is used to size tables by the CMI framework. It is
 *	only filled in for Intel and AMD CPUs.
 *
 * cpi_nthread_bits
 *
 *	This indicates the number of bits required to represent all of the IDs
 *	that cover the logical CPUs that exist on a given core. It's OK for this
 *	value to be larger than the actual number of IDs that are present in the
 *	system.  This is used to size tables by the CMI framework. It is
 *	only filled in for Intel and AMD CPUs.
 *
 * -----------
 * Hypervisors
 * -----------
 *
 * If trying to manage the differences between vendors wasn't bad enough, it can
 * get worse thanks to our friend hardware virtualization. Hypervisors are given
 * the ability to interpose on all cpuid instructions and change them to suit
 * their purposes. In general, this is necessary as the hypervisor wants to be
 * able to present a more uniform set of features or not necessarily give the
 * guest operating system kernel knowledge of all features so it can be
 * more easily migrated between systems.
 *
 * When it comes to trying to determine topology information, this can be a
 * double edged sword. When a hypervisor doesn't actually implement a cpuid
 * leaf, it'll often return all zeros. Because of that, you'll often see various
 * checks scattered about fields being non-zero before we assume we can use
 * them.
 *
 * When it comes to topology information, the hypervisor is often incentivized
 * to lie to you about topology. This is because it doesn't always actually
 * guarantee that topology at all. The topology path we take in the system
 * depends on how the CPU advertises itself. If it advertises itself as an Intel
 * or AMD CPU, then we basically do our normal path. However, when they don't
 * use an actual vendor, then that usually turns into multiple one-core CPUs
 * that we enumerate that are often on different sockets. The actual behavior
 * depends greatly on what the hypervisor actually exposes to us.
 *
 * --------------------
 * Exposing Information
 * --------------------
 *
 * We expose CPUID information in three different forms in the system.
 *
 * The first is through the x86_featureset variable. This is used in conjunction
 * with the is_x86_feature() function. This is queried by x86-specific functions
 * to determine which features are or aren't present in the system and to make
 * decisions based upon them. For example, users of this include everything from
 * parts of the system dedicated to reliability, availability, and
 * serviceability (RAS), to making decisions about how to handle security
 * mitigations, to various x86-specific drivers. General purpose or
 * architecture independent drivers should never be calling this function.
 *
 * The second means is through the auxiliary vector. The auxiliary vector is a
 * series of tagged data that the kernel passes down to a user program when it
 * begins executing. This information is used to indicate to programs what
 * instruction set extensions are present. For example, information about the
 * CPU supporting the machine check architecture (MCA) wouldn't be passed down
 * since user programs cannot make use of it. However, things like the AVX
 * instruction sets are. Programs use this information to make run-time
 * decisions about what features they should use. As an example, the run-time
 * link-editor (rtld) can relocate different functions depending on the hardware
 * support available.
 *
 * The final form is through a series of accessor functions that all have the
 * form cpuid_get*. This is used by a number of different subsystems in the
 * kernel to determine more detailed information about what we're running on,
 * topology information, etc. Some of these subsystems include processor groups
 * (uts/common/os/pg.c.), CPU Module Interface (uts/i86pc/os/cmi.c), ACPI,
 * microcode, and performance monitoring. These functions all ASSERT that the
 * CPU they're being called on has reached a certain cpuid pass. If the passes
 * are rearranged, then this needs to be adjusted.
 *
 * -----------------------------------------------
 * Speculative Execution CPU Side Channel Security
 * -----------------------------------------------
 *
 * With the advent of the Spectre and Meltdown attacks which exploit speculative
 * execution in the CPU to create side channels there have been a number of
 * different attacks and corresponding issues that the operating system needs to
 * mitigate against. The following list is some of the common, but not
 * exhaustive, set of issues that we know about and have done some or need to do
 * more work in the system to mitigate against:
 *
 *   - Spectre v1
 *   - swapgs (Spectre v1 variant)
 *   - Spectre v2
 *   - Meltdown (Spectre v3)
 *   - Rogue Register Read (Spectre v3a)
 *   - Speculative Store Bypass (Spectre v4)
 *   - ret2spec, SpectreRSB
 *   - L1 Terminal Fault (L1TF)
 *   - Microarchitectural Data Sampling (MDS)
 *
 * Each of these requires different sets of mitigations and has different attack
 * surfaces. For the most part, this discussion is about protecting the kernel
 * from non-kernel executing environments such as user processes and hardware
 * virtual machines. Unfortunately, there are a number of user vs. user
 * scenarios that exist with these. The rest of this section will describe the
 * overall approach that the system has taken to address these as well as their
 * shortcomings. Unfortunately, not all of the above have been handled today.
 *
 * SPECTRE v2, ret2spec, SpectreRSB
 *
 * The second variant of the spectre attack focuses on performing branch target
 * injection. This generally impacts indirect call instructions in the system.
 * There are three different ways to mitigate this issue that are commonly
 * described today:
 *
 *  1. Using Indirect Branch Restricted Speculation (IBRS).
 *  2. Using Retpolines and RSB Stuffing
 *  3. Using Enhanced Indirect Branch Restricted Speculation (EIBRS)
 *
 * IBRS uses a feature added to microcode to restrict speculation, among other
 * things. This form of mitigation has not been used as it has been generally
 * seen as too expensive and requires reactivation upon various transitions in
 * the system.
 *
 * As a less impactful alternative to IBRS, retpolines were developed by
 * Google. These basically require one to replace indirect calls with a specific
 * trampoline that will cause speculation to fail and break the attack.
 * Retpolines require compiler support. We always build with retpolines in the
 * external thunk mode. This means that a traditional indirect call is replaced
 * with a call to one of the __x86_indirect_thunk_<reg> functions. A side effect
 * of this is that all indirect function calls are performed through a register.
 *
 * We have to use a common external location of the thunk and not inline it into
 * the callsite so that way we can have a single place to patch these functions.
 * As it turns out, we actually have three different forms of retpolines that
 * exist in the system:
 *
 *  1. A full retpoline
 *  2. An AMD-specific optimized retpoline
 *  3. A no-op version
 *
 * The first one is used in the general case. The second one is used if we can
 * determine that we're on an AMD system and we can successfully toggle the
 * lfence serializing MSR that exists on the platform. Basically with this
 * present, an lfence is sufficient and we don't need to do anywhere near as
 * complicated a dance to successfully use retpolines.
 *
 * The third form described above is the most curious. It turns out that the way
 * that retpolines are implemented is that they rely on how speculation is
 * performed on a 'ret' instruction. Intel has continued to optimize this
 * process (which is partly why we need to have return stack buffer stuffing,
 * but more on that in a bit) and in processors starting with Cascade Lake
 * on the server side, it's dangerous to rely on retpolines. Instead, a new
 * mechanism has been introduced called Enhanced IBRS (EIBRS).
 *
 * Unlike IBRS, EIBRS is designed to be enabled once at boot and left on each
 * physical core. However, if this is the case, we don't want to use retpolines
 * any more. Therefore if EIBRS is present, we end up turning each retpoline
 * function (called a thunk) into a jmp instruction. This means that we're still
 * paying the cost of an extra jump to the external thunk, but it gives us
 * flexibility and the ability to have a single kernel image that works across a
 * wide variety of systems and hardware features.
 *
 * Unfortunately, this alone is insufficient. First, Skylake systems have
 * additional speculation for the Return Stack Buffer (RSB) which is used to
 * return from call instructions which retpolines take advantage of. However,
 * this problem is not just limited to Skylake and is actually more pernicious.
 * The SpectreRSB paper introduces several more problems that can arise with
 * dealing with this. The RSB can be poisoned just like the indirect branch
 * predictor. This means that one needs to clear the RSB when transitioning
 * between two different privilege domains. Some examples include:
 *
 *  - Switching between two different user processes
 *  - Going between user land and the kernel
 *  - Returning to the kernel from a hardware virtual machine
 *
 * Mitigating this involves combining a couple of different things. The first is
 * SMEP (supervisor mode execution protection) which was introduced in Ivy
 * Bridge. When an RSB entry refers to a user address and we're executing in the
 * kernel, speculation through it will be stopped when SMEP is enabled. This
 * protects against a number of the different cases that we would normally be
 * worried about such as when we enter the kernel from user land.
 *
 * To prevent against additional manipulation of the RSB from other contexts
 * such as a non-root VMX context attacking the kernel we first look to enhanced
 * IBRS. When EIBRS is present and enabled, then there is nothing else that we
 * need to do to protect the kernel at this time.
 *
 * On CPUs without EIBRS we need to manually overwrite the contents of the
 * return stack buffer. We do this through the x86_rsb_stuff() function.
 * Currently this is employed on context switch. The x86_rsb_stuff() function is
 * disabled when enhanced IBRS is present because Intel claims on such systems
 * it will be ineffective. Stuffing the RSB in context switch helps prevent user
 * to user attacks via the RSB.
 *
 * If SMEP is not present, then we would have to stuff the RSB every time we
 * transitioned from user mode to the kernel, which isn't very practical right
 * now.
 *
 * To fully protect user to user and vmx to vmx attacks from these classes of
 * issues, we would also need to allow them to opt into performing an Indirect
 * Branch Prediction Barrier (IBPB) on switch. This is not currently wired up.
 *
 * By default, the system will enable RSB stuffing and the required variant of
 * retpolines and store that information in the x86_spectrev2_mitigation value.
 * This will be evaluated after a microcode update as well, though it is
 * expected that microcode updates will not take away features. This may mean
 * that a late loaded microcode may not end up in the optimal configuration
 * (though this should be rare).
 *
 * Currently we do not build kmdb with retpolines or perform any additional side
 * channel security mitigations for it. One complication with kmdb is that it
 * requires its own retpoline thunks and it would need to adjust itself based on
 * what the kernel does. The threat model of kmdb is more limited and therefore
 * it may make more sense to investigate using prediction barriers as the whole
 * system is only executing a single instruction at a time while in kmdb.
 *
 * SPECTRE v1, v4
 *
 * The v1 and v4 variants of spectre are not currently mitigated in the
 * system and require other classes of changes to occur in the code.
 *
 * SPECTRE v1 (SWAPGS VARIANT)
 *
 * The class of Spectre v1 vulnerabilities aren't all about bounds checks, but
 * can generally affect any branch-dependent code. The swapgs issue is one
 * variant of this. If we are coming in from userspace, we can have code like
 * this:
 *
 *	cmpw	$KCS_SEL, REGOFF_CS(%rsp)
 *	je	1f
 *	movq	$0, REGOFF_SAVFP(%rsp)
 *	swapgs
 *	1:
 *	movq	%gs:CPU_THREAD, %rax
 *
 * If an attacker can cause a mis-speculation of the branch here, we could skip
 * the needed swapgs, and use the /user/ %gsbase as the base of the %gs-based
 * load. If subsequent code can act as the usual Spectre cache gadget, this
 * would potentially allow KPTI bypass. To fix this, we need an lfence prior to
 * any use of the %gs override.
 *
 * The other case is also an issue: if we're coming into a trap from kernel
 * space, we could mis-speculate and swapgs the user %gsbase back in prior to
 * using it. AMD systems are not vulnerable to this version, as a swapgs is
 * serializing with respect to subsequent uses. But as AMD /does/ need the other
 * case, and the fix is the same in both cases (an lfence at the branch target
 * 1: in this example), we'll just do it unconditionally.
 *
 * Note that we don't enable user-space "wrgsbase" via CR4_FSGSBASE, making it
 * harder for user-space to actually set a useful %gsbase value: although it's
 * not clear, it might still be feasible via lwp_setprivate(), though, so we
 * mitigate anyway.
 *
 * MELTDOWN
 *
 * Meltdown, or spectre v3, allowed a user process to read any data in their
 * address space regardless of whether or not the page tables in question
 * allowed the user to have the ability to read them. The solution to meltdown
 * is kernel page table isolation. In this world, there are two page tables that
 * are used for a process, one in user land and one in the kernel. To implement
 * this we use per-CPU page tables and switch between the user and kernel
 * variants when entering and exiting the kernel.  For more information about
 * this process and how the trampolines work, please see the big theory
 * statements and additional comments in:
 *
 *  - uts/i86pc/ml/kpti_trampolines.s
 *  - uts/i86pc/vm/hat_i86.c
 *
 * While Meltdown only impacted Intel systems and there are also Intel systems
 * that have Meltdown fixed (called Rogue Data Cache Load), we always have
 * kernel page table isolation enabled. While this may at first seem weird, an
 * important thing to remember is that you can't speculatively read an address
 * if it's never in your page table at all. Having user processes without kernel
 * pages present provides us with an important layer of defense in the kernel
 * against any other side channel attacks that exist and have yet to be
 * discovered. As such, kernel page table isolation (KPTI) is always enabled by
 * default, no matter the x86 system.
 *
 * L1 TERMINAL FAULT
 *
 * L1 Terminal Fault (L1TF) takes advantage of an issue in how speculative
 * execution uses page table entries. Effectively, it is two different problems.
 * The first is that it ignores the not present bit in the page table entries
 * when performing speculative execution. This means that something can
 * speculatively read the listed physical address if it's present in the L1
 * cache under certain conditions (see Intel's documentation for the full set of
 * conditions). Secondly, this can be used to bypass hardware virtualization
 * extended page tables (EPT) that are part of Intel's hardware virtual machine
 * instructions.
 *
 * For the non-hardware virtualized case, this is relatively easy to deal with.
 * We must make sure that all unmapped pages have an address of zero. This means
 * that they could read the first 4k of physical memory; however, we never use
 * that first page in the operating system and always skip putting it in our
 * memory map, even if firmware tells us we can use it in our memory map. While
 * other systems try to put extra metadata in the address and reserved bits,
 * which led to this being problematic in those cases, we do not.
 *
 * For hardware virtual machines things are more complicated. Because they can
 * construct their own page tables, it isn't hard for them to perform this
 * attack against any physical address. The one wrinkle is that this physical
 * address must be in the L1 data cache. Thus Intel added an MSR that we can use
 * to flush the L1 data cache. We wrap this up in the function
 * spec_uarch_flush(). This function is also used in the mitigation of
 * microarchitectural data sampling (MDS) discussed later on. Kernel based
 * hypervisors such as KVM or bhyve are responsible for performing this before
 * entering the guest.
 *
 * Because this attack takes place in the L1 cache, there's another wrinkle
 * here. The L1 cache is shared between all logical CPUs in a core in most Intel
 * designs. This means that when a thread enters a hardware virtualized context
 * and flushes the L1 data cache, the other thread on the processor may then go
 * ahead and put new data in it that can be potentially attacked. While one
 * solution is to disable SMT on the system, another option that is available is
 * to use a feature for hardware virtualization called 'SMT exclusion'. This
 * goes through and makes sure that if a HVM is being scheduled on one thread,
 * then the thing on the other thread is from the same hardware virtual machine.
 * If an interrupt comes in or the guest exits to the broader system, then the
 * other SMT thread will be kicked out.
 *
 * L1TF can be fully mitigated by hardware. If the RDCL_NO feature is set in the
 * architecture capabilities MSR (MSR_IA32_ARCH_CAPABILITIES), then we will not
 * perform L1TF related mitigations.
 *
 * MICROARCHITECTURAL DATA SAMPLING
 *
 * Microarchitectural data sampling (MDS) is a combination of four discrete
 * vulnerabilities that are similar issues affecting various parts of the CPU's
 * microarchitectural implementation around load, store, and fill buffers.
 * Specifically it is made up of the following subcomponents:
 *
 *  1. Microarchitectural Store Buffer Data Sampling (MSBDS)
 *  2. Microarchitectural Fill Buffer Data Sampling (MFBDS)
 *  3. Microarchitectural Load Port Data Sampling (MLPDS)
 *  4. Microarchitectural Data Sampling Uncacheable Memory (MDSUM)
 *
 * To begin addressing these, Intel has introduced another feature in microcode
 * called MD_CLEAR. This changes the verw instruction to operate in a different
 * way. This allows us to execute the verw instruction in a particular way to
 * flush the state of the affected parts. The L1TF L1D flush mechanism is also
 * updated when this microcode is present to flush this state.
 *
 * Primarily we need to flush this state whenever we transition from the kernel
 * to a less privileged context such as user mode or an HVM guest. MSBDS is a
 * little bit different. Here the structures are statically sized when a logical
 * CPU is in use and resized when it goes to sleep. Therefore, we also need to
 * flush the microarchitectural state before the CPU goes idles by calling hlt,
 * mwait, or another ACPI method. To perform these flushes, we call
 * x86_md_clear() at all of these transition points.
 *
 * If hardware enumerates RDCL_NO, indicating that it is not vulnerable to L1TF,
 * then we change the spec_uarch_flush() function to point to x86_md_clear(). If
 * MDS_NO has been set, then this is fully mitigated and x86_md_clear() becomes
 * a no-op.
 *
 * Unfortunately, with this issue hyperthreading rears its ugly head. In
 * particular, everything we've discussed above is only valid for a single
 * thread executing on a core. In the case where you have hyper-threading
 * present, this attack can be performed between threads. The theoretical fix
 * for this is to ensure that both threads are always in the same security
 * domain. This means that they are executing in the same ring and mutually
 * trust each other. Practically speaking, this would mean that a system call
 * would have to issue an inter-processor interrupt (IPI) to the other thread.
 * Rather than implement this, we recommend that one disables hyper-threading
 * through the use of psradm -aS.
 *
 * SUMMARY
 *
 * The following table attempts to summarize the mitigations for various issues
 * and what's done in various places:
 *
 *  - Spectre v1: Not currently mitigated
 *  - swapgs: lfences after swapgs paths
 *  - Spectre v2: Retpolines/RSB Stuffing or EIBRS if HW support
 *  - Meltdown: Kernel Page Table Isolation
 *  - Spectre v3a: Updated CPU microcode
 *  - Spectre v4: Not currently mitigated
 *  - SpectreRSB: SMEP and RSB Stuffing
 *  - L1TF: spec_uarch_flush, SMT exclusion, requires microcode
 *  - MDS: x86_md_clear, requires microcode, disabling hyper threading
 *
 * The following table indicates the x86 feature set bits that indicate that a
 * given problem has been solved or a notable feature is present:
 *
 *  - RDCL_NO: Meltdown, L1TF, MSBDS subset of MDS
 *  - MDS_NO: All forms of MDS
 */

#include <sys/types.h>
#include <sys/archsystm.h>
#include <sys/x86_archext.h>
#include <sys/kmem.h>
#include <sys/systm.h>
#include <sys/cmn_err.h>
#include <sys/sunddi.h>
#include <sys/sunndi.h>
#include <sys/cpuvar.h>
#include <sys/processor.h>
#include <sys/sysmacros.h>
#include <sys/pg.h>
#include <sys/fp.h>
#include <sys/controlregs.h>
#include <sys/bitmap.h>
#include <sys/auxv_386.h>
#include <sys/memnode.h>
#include <sys/pci_cfgspace.h>
#include <sys/comm_page.h>
#include <sys/mach_mmu.h>
#include <sys/ucode.h>
#include <sys/tsc.h>
#include <sys/kobj.h>
#include <sys/asm_misc.h>

#ifdef __xpv
#include <sys/hypervisor.h>
#else
#include <sys/ontrap.h>
#endif

uint_t x86_vendor = X86_VENDOR_IntelClone;
uint_t x86_type = X86_TYPE_OTHER;
uint_t x86_clflush_size = 0;

#if defined(__xpv)
int x86_use_pcid = 0;
int x86_use_invpcid = 0;
#else
int x86_use_pcid = -1;
int x86_use_invpcid = -1;
#endif

typedef enum {
	X86_SPECTREV2_RETPOLINE,
	X86_SPECTREV2_RETPOLINE_AMD,
	X86_SPECTREV2_ENHANCED_IBRS,
	X86_SPECTREV2_DISABLED
} x86_spectrev2_mitigation_t;

uint_t x86_disable_spectrev2 = 0;
static x86_spectrev2_mitigation_t x86_spectrev2_mitigation =
    X86_SPECTREV2_RETPOLINE;

uint_t pentiumpro_bug4046376;

uchar_t x86_featureset[BT_SIZEOFMAP(NUM_X86_FEATURES)];

static char *x86_feature_names[NUM_X86_FEATURES] = {
	"lgpg",
	"tsc",
	"msr",
	"mtrr",
	"pge",
	"de",
	"cmov",
	"mmx",
	"mca",
	"pae",
	"cv8",
	"pat",
	"sep",
	"sse",
	"sse2",
	"htt",
	"asysc",
	"nx",
	"sse3",
	"cx16",
	"cmp",
	"tscp",
	"mwait",
	"sse4a",
	"cpuid",
	"ssse3",
	"sse4_1",
	"sse4_2",
	"1gpg",
	"clfsh",
	"64",
	"aes",
	"pclmulqdq",
	"xsave",
	"avx",
	"vmx",
	"svm",
	"topoext",
	"f16c",
	"rdrand",
	"x2apic",
	"avx2",
	"bmi1",
	"bmi2",
	"fma",
	"smep",
	"smap",
	"adx",
	"rdseed",
	"mpx",
	"avx512f",
	"avx512dq",
	"avx512pf",
	"avx512er",
	"avx512cd",
	"avx512bw",
	"avx512vl",
	"avx512fma",
	"avx512vbmi",
	"avx512_vpopcntdq",
	"avx512_4vnniw",
	"avx512_4fmaps",
	"xsaveopt",
	"xsavec",
	"xsaves",
	"sha",
	"umip",
	"pku",
	"ospke",
	"pcid",
	"invpcid",
	"ibrs",
	"ibpb",
	"stibp",
	"ssbd",
	"ssbd_virt",
	"rdcl_no",
	"ibrs_all",
	"rsba",
	"ssb_no",
	"stibp_all",
	"flush_cmd",
	"l1d_vmentry_no",
	"fsgsbase",
	"clflushopt",
	"clwb",
	"monitorx",
	"clzero",
	"xop",
	"fma4",
	"tbm",
	"avx512_vnni",
	"amd_pcec",
	"mb_clear",
	"mds_no",
	"core_thermal",
	"pkg_thermal"
};

boolean_t
is_x86_feature(void *featureset, uint_t feature)
{
	ASSERT(feature < NUM_X86_FEATURES);
	return (BT_TEST((ulong_t *)featureset, feature));
}

void
add_x86_feature(void *featureset, uint_t feature)
{
	ASSERT(feature < NUM_X86_FEATURES);
	BT_SET((ulong_t *)featureset, feature);
}

void
remove_x86_feature(void *featureset, uint_t feature)
{
	ASSERT(feature < NUM_X86_FEATURES);
	BT_CLEAR((ulong_t *)featureset, feature);
}

boolean_t
compare_x86_featureset(void *setA, void *setB)
{
	/*
	 * We assume that the unused bits of the bitmap are always zero.
	 */
	if (memcmp(setA, setB, BT_SIZEOFMAP(NUM_X86_FEATURES)) == 0) {
		return (B_TRUE);
	} else {
		return (B_FALSE);
	}
}

void
print_x86_featureset(void *featureset)
{
	uint_t i;

	for (i = 0; i < NUM_X86_FEATURES; i++) {
		if (is_x86_feature(featureset, i)) {
			cmn_err(CE_CONT, "?x86_feature: %s\n",
			    x86_feature_names[i]);
		}
	}
}

/* Note: This is the maximum size for the CPU, not the size of the structure. */
static size_t xsave_state_size = 0;
uint64_t xsave_bv_all = (XFEATURE_LEGACY_FP | XFEATURE_SSE);
boolean_t xsave_force_disable = B_FALSE;
extern int disable_smap;

/*
 * This is set to platform type we are running on.
 */
static int platform_type = -1;

#if !defined(__xpv)
/*
 * Variable to patch if hypervisor platform detection needs to be
 * disabled (e.g. platform_type will always be HW_NATIVE if this is 0).
 */
int enable_platform_detection = 1;
#endif

/*
 * monitor/mwait info.
 *
 * size_actual and buf_actual are the real address and size allocated to get
 * proper mwait_buf alignement.  buf_actual and size_actual should be passed
 * to kmem_free().  Currently kmem_alloc() and mwait happen to both use
 * processor cache-line alignment, but this is not guarantied in the furture.
 */
struct mwait_info {
	size_t		mon_min;	/* min size to avoid missed wakeups */
	size_t		mon_max;	/* size to avoid false wakeups */
	size_t		size_actual;	/* size actually allocated */
	void		*buf_actual;	/* memory actually allocated */
	uint32_t	support;	/* processor support of monitor/mwait */
};

/*
 * xsave/xrestor info.
 *
 * This structure contains HW feature bits and the size of the xsave save area.
 * Note: the kernel declares a fixed size (AVX_XSAVE_SIZE) structure
 * (xsave_state) to describe the xsave layout. However, at runtime the
 * per-lwp xsave area is dynamically allocated based on xsav_max_size. The
 * xsave_state structure simply represents the legacy layout of the beginning
 * of the xsave area.
 */
struct xsave_info {
	uint32_t	xsav_hw_features_low;   /* Supported HW features */
	uint32_t	xsav_hw_features_high;  /* Supported HW features */
	size_t		xsav_max_size;  /* max size save area for HW features */
	size_t		ymm_size;	/* AVX: size of ymm save area */
	size_t		ymm_offset;	/* AVX: offset for ymm save area */
	size_t		bndregs_size;	/* MPX: size of bndregs save area */
	size_t		bndregs_offset;	/* MPX: offset for bndregs save area */
	size_t		bndcsr_size;	/* MPX: size of bndcsr save area */
	size_t		bndcsr_offset;	/* MPX: offset for bndcsr save area */
	size_t		opmask_size;	/* AVX512: size of opmask save */
	size_t		opmask_offset;	/* AVX512: offset for opmask save */
	size_t		zmmlo_size;	/* AVX512: size of zmm 256 save */
	size_t		zmmlo_offset;	/* AVX512: offset for zmm 256 save */
	size_t		zmmhi_size;	/* AVX512: size of zmm hi reg save */
	size_t		zmmhi_offset;	/* AVX512: offset for zmm hi reg save */
};


/*
 * These constants determine how many of the elements of the
 * cpuid we cache in the cpuid_info data structure; the
 * remaining elements are accessible via the cpuid instruction.
 */

#define	NMAX_CPI_STD	8		/* eax = 0 .. 7 */
#define	NMAX_CPI_EXTD	0x1f		/* eax = 0x80000000 .. 0x8000001e */

/*
 * See the big theory statement for a more detailed explanation of what some of
 * these members mean.
 */
struct cpuid_info {
	uint_t cpi_pass;		/* last pass completed */
	/*
	 * standard function information
	 */
	uint_t cpi_maxeax;		/* fn 0: %eax */
	char cpi_vendorstr[13];		/* fn 0: %ebx:%ecx:%edx */
	uint_t cpi_vendor;		/* enum of cpi_vendorstr */

	uint_t cpi_family;		/* fn 1: extended family */
	uint_t cpi_model;		/* fn 1: extended model */
	uint_t cpi_step;		/* fn 1: stepping */
	chipid_t cpi_chipid;		/* fn 1: %ebx:  Intel: chip # */
					/*		AMD: package/socket # */
	uint_t cpi_brandid;		/* fn 1: %ebx: brand ID */
	int cpi_clogid;			/* fn 1: %ebx: thread # */
	uint_t cpi_ncpu_per_chip;	/* fn 1: %ebx: logical cpu count */
	uint8_t cpi_cacheinfo[16];	/* fn 2: intel-style cache desc */
	uint_t cpi_ncache;		/* fn 2: number of elements */
	uint_t cpi_ncpu_shr_last_cache;	/* fn 4: %eax: ncpus sharing cache */
	id_t cpi_last_lvl_cacheid;	/* fn 4: %eax: derived cache id */
	uint_t cpi_cache_leaf_size;	/* Number of cache elements */
					/* Intel fn: 4, AMD fn: 8000001d */
	struct cpuid_regs **cpi_cache_leaves;	/* Acual leaves from above */
	struct cpuid_regs cpi_std[NMAX_CPI_STD];	/* 0 .. 7 */
	/*
	 * extended function information
	 */
	uint_t cpi_xmaxeax;		/* fn 0x80000000: %eax */
	char cpi_brandstr[49];		/* fn 0x8000000[234] */
	uint8_t cpi_pabits;		/* fn 0x80000006: %eax */
	uint8_t	cpi_vabits;		/* fn 0x80000006: %eax */
	uint8_t cpi_fp_amd_save;	/* AMD: FP error pointer save rqd. */
	struct	cpuid_regs cpi_extd[NMAX_CPI_EXTD];	/* 0x800000XX */

	id_t cpi_coreid;		/* same coreid => strands share core */
	int cpi_pkgcoreid;		/* core number within single package */
	uint_t cpi_ncore_per_chip;	/* AMD: fn 0x80000008: %ecx[7-0] */
					/* Intel: fn 4: %eax[31-26] */

	/*
	 * These values represent the number of bits that are required to store
	 * information about the number of cores and threads.
	 */
	uint_t cpi_ncore_bits;
	uint_t cpi_nthread_bits;
	/*
	 * supported feature information
	 */
	uint32_t cpi_support[6];
#define	STD_EDX_FEATURES	0
#define	AMD_EDX_FEATURES	1
#define	TM_EDX_FEATURES		2
#define	STD_ECX_FEATURES	3
#define	AMD_ECX_FEATURES	4
#define	STD_EBX_FEATURES	5
	/*
	 * Synthesized information, where known.
	 */
	uint32_t cpi_chiprev;		/* See X86_CHIPREV_* in x86_archext.h */
	const char *cpi_chiprevstr;	/* May be NULL if chiprev unknown */
	uint32_t cpi_socket;		/* Chip package/socket type */

	struct mwait_info cpi_mwait;	/* fn 5: monitor/mwait info */
	uint32_t cpi_apicid;
	uint_t cpi_procnodeid;		/* AMD: nodeID on HT, Intel: chipid */
	uint_t cpi_procnodes_per_pkg;	/* AMD: # of nodes in the package */
					/* Intel: 1 */
	uint_t cpi_compunitid;		/* AMD: ComputeUnit ID, Intel: coreid */
	uint_t cpi_cores_per_compunit;	/* AMD: # of cores in the ComputeUnit */

	struct xsave_info cpi_xsave;	/* fn D: xsave/xrestor info */
};


static struct cpuid_info cpuid_info0;

/*
 * These bit fields are defined by the Intel Application Note AP-485
 * "Intel Processor Identification and the CPUID Instruction"
 */
#define	CPI_FAMILY_XTD(cpi)	BITX((cpi)->cpi_std[1].cp_eax, 27, 20)
#define	CPI_MODEL_XTD(cpi)	BITX((cpi)->cpi_std[1].cp_eax, 19, 16)
#define	CPI_TYPE(cpi)		BITX((cpi)->cpi_std[1].cp_eax, 13, 12)
#define	CPI_FAMILY(cpi)		BITX((cpi)->cpi_std[1].cp_eax, 11, 8)
#define	CPI_STEP(cpi)		BITX((cpi)->cpi_std[1].cp_eax, 3, 0)
#define	CPI_MODEL(cpi)		BITX((cpi)->cpi_std[1].cp_eax, 7, 4)

#define	CPI_FEATURES_EDX(cpi)		((cpi)->cpi_std[1].cp_edx)
#define	CPI_FEATURES_ECX(cpi)		((cpi)->cpi_std[1].cp_ecx)
#define	CPI_FEATURES_XTD_EDX(cpi)	((cpi)->cpi_extd[1].cp_edx)
#define	CPI_FEATURES_XTD_ECX(cpi)	((cpi)->cpi_extd[1].cp_ecx)
#define	CPI_FEATURES_7_0_EBX(cpi)	((cpi)->cpi_std[7].cp_ebx)
#define	CPI_FEATURES_7_0_ECX(cpi)	((cpi)->cpi_std[7].cp_ecx)
#define	CPI_FEATURES_7_0_EDX(cpi)	((cpi)->cpi_std[7].cp_edx)

#define	CPI_BRANDID(cpi)	BITX((cpi)->cpi_std[1].cp_ebx, 7, 0)
#define	CPI_CHUNKS(cpi)		BITX((cpi)->cpi_std[1].cp_ebx, 15, 7)
#define	CPI_CPU_COUNT(cpi)	BITX((cpi)->cpi_std[1].cp_ebx, 23, 16)
#define	CPI_APIC_ID(cpi)	BITX((cpi)->cpi_std[1].cp_ebx, 31, 24)

#define	CPI_MAXEAX_MAX		0x100		/* sanity control */
#define	CPI_XMAXEAX_MAX		0x80000100
#define	CPI_FN4_ECX_MAX		0x20		/* sanity: max fn 4 levels */
#define	CPI_FNB_ECX_MAX		0x20		/* sanity: max fn B levels */

/*
 * Function 4 (Deterministic Cache Parameters) macros
 * Defined by Intel Application Note AP-485
 */
#define	CPI_NUM_CORES(regs)		BITX((regs)->cp_eax, 31, 26)
#define	CPI_NTHR_SHR_CACHE(regs)	BITX((regs)->cp_eax, 25, 14)
#define	CPI_FULL_ASSOC_CACHE(regs)	BITX((regs)->cp_eax, 9, 9)
#define	CPI_SELF_INIT_CACHE(regs)	BITX((regs)->cp_eax, 8, 8)
#define	CPI_CACHE_LVL(regs)		BITX((regs)->cp_eax, 7, 5)
#define	CPI_CACHE_TYPE(regs)		BITX((regs)->cp_eax, 4, 0)
#define	CPI_CPU_LEVEL_TYPE(regs)	BITX((regs)->cp_ecx, 15, 8)

#define	CPI_CACHE_WAYS(regs)		BITX((regs)->cp_ebx, 31, 22)
#define	CPI_CACHE_PARTS(regs)		BITX((regs)->cp_ebx, 21, 12)
#define	CPI_CACHE_COH_LN_SZ(regs)	BITX((regs)->cp_ebx, 11, 0)

#define	CPI_CACHE_SETS(regs)		BITX((regs)->cp_ecx, 31, 0)

#define	CPI_PREFCH_STRIDE(regs)		BITX((regs)->cp_edx, 9, 0)


/*
 * A couple of shorthand macros to identify "later" P6-family chips
 * like the Pentium M and Core.  First, the "older" P6-based stuff
 * (loosely defined as "pre-Pentium-4"):
 * P6, PII, Mobile PII, PII Xeon, PIII, Mobile PIII, PIII Xeon
 */
#define	IS_LEGACY_P6(cpi) (			\
	cpi->cpi_family == 6 &&			\
		(cpi->cpi_model == 1 ||		\
		cpi->cpi_model == 3 ||		\
		cpi->cpi_model == 5 ||		\
		cpi->cpi_model == 6 ||		\
		cpi->cpi_model == 7 ||		\
		cpi->cpi_model == 8 ||		\
		cpi->cpi_model == 0xA ||	\
		cpi->cpi_model == 0xB)		\
)

/* A "new F6" is everything with family 6 that's not the above */
#define	IS_NEW_F6(cpi) ((cpi->cpi_family == 6) && !IS_LEGACY_P6(cpi))

/* Extended family/model support */
#define	IS_EXTENDED_MODEL_INTEL(cpi) (cpi->cpi_family == 0x6 || \
	cpi->cpi_family >= 0xf)

/*
 * Info for monitor/mwait idle loop.
 *
 * See cpuid section of "Intel 64 and IA-32 Architectures Software Developer's
 * Manual Volume 2A: Instruction Set Reference, A-M" #25366-022US, November
 * 2006.
 * See MONITOR/MWAIT section of "AMD64 Architecture Programmer's Manual
 * Documentation Updates" #33633, Rev 2.05, December 2006.
 */
#define	MWAIT_SUPPORT		(0x00000001)	/* mwait supported */
#define	MWAIT_EXTENSIONS	(0x00000002)	/* extenstion supported */
#define	MWAIT_ECX_INT_ENABLE	(0x00000004)	/* ecx 1 extension supported */
#define	MWAIT_SUPPORTED(cpi)	((cpi)->cpi_std[1].cp_ecx & CPUID_INTC_ECX_MON)
#define	MWAIT_INT_ENABLE(cpi)	((cpi)->cpi_std[5].cp_ecx & 0x2)
#define	MWAIT_EXTENSION(cpi)	((cpi)->cpi_std[5].cp_ecx & 0x1)
#define	MWAIT_SIZE_MIN(cpi)	BITX((cpi)->cpi_std[5].cp_eax, 15, 0)
#define	MWAIT_SIZE_MAX(cpi)	BITX((cpi)->cpi_std[5].cp_ebx, 15, 0)
/*
 * Number of sub-cstates for a given c-state.
 */
#define	MWAIT_NUM_SUBC_STATES(cpi, c_state)			\
	BITX((cpi)->cpi_std[5].cp_edx, c_state + 3, c_state)

/*
 * XSAVE leaf 0xD enumeration
 */
#define	CPUID_LEAFD_2_YMM_OFFSET	576
#define	CPUID_LEAFD_2_YMM_SIZE		256

/*
 * Common extended leaf names to cut down on typos.
 */
#define	CPUID_LEAF_EXT_0		0x80000000
#define	CPUID_LEAF_EXT_8		0x80000008
#define	CPUID_LEAF_EXT_1d		0x8000001d
#define	CPUID_LEAF_EXT_1e		0x8000001e

/*
 * Functions we consune from cpuid_subr.c;  don't publish these in a header
 * file to try and keep people using the expected cpuid_* interfaces.
 */
extern uint32_t _cpuid_skt(uint_t, uint_t, uint_t, uint_t);
extern const char *_cpuid_sktstr(uint_t, uint_t, uint_t, uint_t);
extern uint32_t _cpuid_chiprev(uint_t, uint_t, uint_t, uint_t);
extern const char *_cpuid_chiprevstr(uint_t, uint_t, uint_t, uint_t);
extern uint_t _cpuid_vendorstr_to_vendorcode(char *);

/*
 * Apply up various platform-dependent restrictions where the
 * underlying platform restrictions mean the CPU can be marked
 * as less capable than its cpuid instruction would imply.
 */
#if defined(__xpv)
static void
platform_cpuid_mangle(uint_t vendor, uint32_t eax, struct cpuid_regs *cp)
{
	switch (eax) {
	case 1: {
		uint32_t mcamask = DOMAIN_IS_INITDOMAIN(xen_info) ?
		    0 : CPUID_INTC_EDX_MCA;
		cp->cp_edx &=
		    ~(mcamask |
		    CPUID_INTC_EDX_PSE |
		    CPUID_INTC_EDX_VME | CPUID_INTC_EDX_DE |
		    CPUID_INTC_EDX_SEP | CPUID_INTC_EDX_MTRR |
		    CPUID_INTC_EDX_PGE | CPUID_INTC_EDX_PAT |
		    CPUID_AMD_EDX_SYSC | CPUID_INTC_EDX_SEP |
		    CPUID_INTC_EDX_PSE36 | CPUID_INTC_EDX_HTT);
		break;
	}

	case 0x80000001:
		cp->cp_edx &=
		    ~(CPUID_AMD_EDX_PSE |
		    CPUID_INTC_EDX_VME | CPUID_INTC_EDX_DE |
		    CPUID_AMD_EDX_MTRR | CPUID_AMD_EDX_PGE |
		    CPUID_AMD_EDX_PAT | CPUID_AMD_EDX_PSE36 |
		    CPUID_AMD_EDX_SYSC | CPUID_INTC_EDX_SEP |
		    CPUID_AMD_EDX_TSCP);
		cp->cp_ecx &= ~CPUID_AMD_ECX_CMP_LGCY;
		break;
	default:
		break;
	}

	switch (vendor) {
	case X86_VENDOR_Intel:
		switch (eax) {
		case 4:
			/*
			 * Zero out the (ncores-per-chip - 1) field
			 */
			cp->cp_eax &= 0x03fffffff;
			break;
		default:
			break;
		}
		break;
	case X86_VENDOR_AMD:
		switch (eax) {

		case 0x80000001:
			cp->cp_ecx &= ~CPUID_AMD_ECX_CR8D;
			break;

		case CPUID_LEAF_EXT_8:
			/*
			 * Zero out the (ncores-per-chip - 1) field
			 */
			cp->cp_ecx &= 0xffffff00;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
#else
#define	platform_cpuid_mangle(vendor, eax, cp)	/* nothing */
#endif

/*
 *  Some undocumented ways of patching the results of the cpuid
 *  instruction to permit running Solaris 10 on future cpus that
 *  we don't currently support.  Could be set to non-zero values
 *  via settings in eeprom.
 */

uint32_t cpuid_feature_ecx_include;
uint32_t cpuid_feature_ecx_exclude;
uint32_t cpuid_feature_edx_include;
uint32_t cpuid_feature_edx_exclude;

/*
 * Allocate space for mcpu_cpi in the machcpu structure for all non-boot CPUs.
 */
void
cpuid_alloc_space(cpu_t *cpu)
{
	/*
	 * By convention, cpu0 is the boot cpu, which is set up
	 * before memory allocation is available.  All other cpus get
	 * their cpuid_info struct allocated here.
	 */
	ASSERT(cpu->cpu_id != 0);
	ASSERT(cpu->cpu_m.mcpu_cpi == NULL);
	cpu->cpu_m.mcpu_cpi =
	    kmem_zalloc(sizeof (*cpu->cpu_m.mcpu_cpi), KM_SLEEP);
}

void
cpuid_free_space(cpu_t *cpu)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	int i;

	ASSERT(cpi != NULL);
	ASSERT(cpi != &cpuid_info0);

	/*
	 * Free up any cache leaf related dynamic storage. The first entry was
	 * cached from the standard cpuid storage, so we should not free it.
	 */
	for (i = 1; i < cpi->cpi_cache_leaf_size; i++)
		kmem_free(cpi->cpi_cache_leaves[i], sizeof (struct cpuid_regs));
	if (cpi->cpi_cache_leaf_size > 0)
		kmem_free(cpi->cpi_cache_leaves,
		    cpi->cpi_cache_leaf_size * sizeof (struct cpuid_regs *));

	kmem_free(cpi, sizeof (*cpi));
	cpu->cpu_m.mcpu_cpi = NULL;
}

#if !defined(__xpv)
/*
 * Determine the type of the underlying platform. This is used to customize
 * initialization of various subsystems (e.g. TSC). determine_platform() must
 * only ever be called once to prevent two processors from seeing different
 * values of platform_type. Must be called before cpuid_pass1(), the earliest
 * consumer to execute (uses _cpuid_chiprev --> synth_amd_info --> get_hwenv).
 */
void
determine_platform(void)
{
	struct cpuid_regs cp;
	uint32_t base;
	uint32_t regs[4];
	char *hvstr = (char *)regs;

	ASSERT(platform_type == -1);

	platform_type = HW_NATIVE;

	if (!enable_platform_detection)
		return;

	/*
	 * If Hypervisor CPUID bit is set, try to determine hypervisor
	 * vendor signature, and set platform type accordingly.
	 *
	 * References:
	 * http://lkml.org/lkml/2008/10/1/246
	 * http://kb.vmware.com/kb/1009458
	 */
	cp.cp_eax = 0x1;
	(void) __cpuid_insn(&cp);
	if ((cp.cp_ecx & CPUID_INTC_ECX_HV) != 0) {
		cp.cp_eax = 0x40000000;
		(void) __cpuid_insn(&cp);
		regs[0] = cp.cp_ebx;
		regs[1] = cp.cp_ecx;
		regs[2] = cp.cp_edx;
		regs[3] = 0;
		if (strcmp(hvstr, HVSIG_XEN_HVM) == 0) {
			platform_type = HW_XEN_HVM;
			return;
		}
		if (strcmp(hvstr, HVSIG_VMWARE) == 0) {
			platform_type = HW_VMWARE;
			return;
		}
		if (strcmp(hvstr, HVSIG_KVM) == 0) {
			platform_type = HW_KVM;
			return;
		}
		if (strcmp(hvstr, HVSIG_BHYVE) == 0) {
			platform_type = HW_BHYVE;
			return;
		}
		if (strcmp(hvstr, HVSIG_MICROSOFT) == 0)
			platform_type = HW_MICROSOFT;
	} else {
		/*
		 * Check older VMware hardware versions. VMware hypervisor is
		 * detected by performing an IN operation to VMware hypervisor
		 * port and checking that value returned in %ebx is VMware
		 * hypervisor magic value.
		 *
		 * References: http://kb.vmware.com/kb/1009458
		 */
		vmware_port(VMWARE_HVCMD_GETVERSION, regs);
		if (regs[1] == VMWARE_HVMAGIC) {
			platform_type = HW_VMWARE;
			return;
		}
	}

	/*
	 * Check Xen hypervisor. In a fully virtualized domain,
	 * Xen's pseudo-cpuid function returns a string representing the
	 * Xen signature in %ebx, %ecx, and %edx. %eax contains the maximum
	 * supported cpuid function. We need at least a (base + 2) leaf value
	 * to do what we want to do. Try different base values, since the
	 * hypervisor might use a different one depending on whether Hyper-V
	 * emulation is switched on by default or not.
	 */
	for (base = 0x40000000; base < 0x40010000; base += 0x100) {
		cp.cp_eax = base;
		(void) __cpuid_insn(&cp);
		regs[0] = cp.cp_ebx;
		regs[1] = cp.cp_ecx;
		regs[2] = cp.cp_edx;
		regs[3] = 0;
		if (strcmp(hvstr, HVSIG_XEN_HVM) == 0 &&
		    cp.cp_eax >= (base + 2)) {
			platform_type &= ~HW_NATIVE;
			platform_type |= HW_XEN_HVM;
			return;
		}
	}
}

int
get_hwenv(void)
{
	ASSERT(platform_type != -1);
	return (platform_type);
}

int
is_controldom(void)
{
	return (0);
}

#else

int
get_hwenv(void)
{
	return (HW_XEN_PV);
}

int
is_controldom(void)
{
	return (DOMAIN_IS_INITDOMAIN(xen_info));
}

#endif	/* __xpv */

/*
 * Make sure that we have gathered all of the CPUID leaves that we might need to
 * determine topology. We assume that the standard leaf 1 has already been done
 * and that xmaxeax has already been calculated.
 */
static void
cpuid_gather_amd_topology_leaves(cpu_t *cpu)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	if (cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_8) {
		struct cpuid_regs *cp;

		cp = &cpi->cpi_extd[8];
		cp->cp_eax = CPUID_LEAF_EXT_8;
		(void) __cpuid_insn(cp);
		platform_cpuid_mangle(cpi->cpi_vendor, CPUID_LEAF_EXT_8, cp);
	}

	if (is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e) {
		struct cpuid_regs *cp;

		cp = &cpi->cpi_extd[0x1e];
		cp->cp_eax = CPUID_LEAF_EXT_1e;
		(void) __cpuid_insn(cp);
	}
}

/*
 * Get the APIC ID for this processor. If Leaf B is present and valid, we prefer
 * it to everything else. If not, and we're on an AMD system where 8000001e is
 * valid, then we use that. Othewrise, we fall back to the default value for the
 * APIC ID in leaf 1.
 */
static uint32_t
cpuid_gather_apicid(struct cpuid_info *cpi)
{
	/*
	 * Leaf B changes based on the arguments to it. Beacuse we don't cache
	 * it, we need to gather it again.
	 */
	if (cpi->cpi_maxeax >= 0xB) {
		struct cpuid_regs regs;
		struct cpuid_regs *cp;

		cp = &regs;
		cp->cp_eax = 0xB;
		cp->cp_edx = cp->cp_ebx = cp->cp_ecx = 0;
		(void) __cpuid_insn(cp);

		if (cp->cp_ebx != 0) {
			return (cp->cp_edx);
		}
	}

	if (cpi->cpi_vendor == X86_VENDOR_AMD &&
	    is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e) {
		return (cpi->cpi_extd[0x1e].cp_eax);
	}

	return (CPI_APIC_ID(cpi));
}

/*
 * For AMD processors, attempt to calculate the number of chips and cores that
 * exist. The way that we do this varies based on the generation, because the
 * generations themselves have changed dramatically.
 *
 * If cpuid leaf 0x80000008 exists, that generally tells us the number of cores.
 * However, with the advent of family 17h (Zen) it actually tells us the number
 * of threads, so we need to look at leaf 0x8000001e if available to determine
 * its value. Otherwise, for all prior families, the number of enabled cores is
 * the same as threads.
 *
 * If we do not have leaf 0x80000008, then we assume that this processor does
 * not have anything. AMD's older CPUID specification says there's no reason to
 * fall back to leaf 1.
 *
 * In some virtualization cases we will not have leaf 8000001e or it will be
 * zero. When that happens we assume the number of threads is one.
 */
static void
cpuid_amd_ncores(struct cpuid_info *cpi, uint_t *ncpus, uint_t *ncores)
{
	uint_t nthreads, nthread_per_core;

	nthreads = nthread_per_core = 1;

	if (cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_8) {
		nthreads = BITX(cpi->cpi_extd[8].cp_ecx, 7, 0) + 1;
	} else if ((cpi->cpi_std[1].cp_edx & CPUID_INTC_EDX_HTT) != 0) {
		nthreads = CPI_CPU_COUNT(cpi);
	}

	/*
	 * For us to have threads, and know about it, we have to be at least at
	 * family 17h and have the cpuid bit that says we have extended
	 * topology.
	 */
	if (cpi->cpi_family >= 0x17 &&
	    is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e) {
		nthread_per_core = BITX(cpi->cpi_extd[0x1e].cp_ebx, 15, 8) + 1;
	}

	*ncpus = nthreads;
	*ncores = nthreads / nthread_per_core;
}

/*
 * Seed the initial values for the cores and threads for an Intel based
 * processor. These values will be overwritten if we detect that the processor
 * supports CPUID leaf 0xb.
 */
static void
cpuid_intel_ncores(struct cpuid_info *cpi, uint_t *ncpus, uint_t *ncores)
{
	/*
	 * Only seed the number of physical cores from the first level leaf 4
	 * information. The number of threads there indicate how many share the
	 * L1 cache, which may or may not have anything to do with the number of
	 * logical CPUs per core.
	 */
	if (cpi->cpi_maxeax >= 4) {
		*ncores = BITX(cpi->cpi_std[4].cp_eax, 31, 26) + 1;
	} else {
		*ncores = 1;
	}

	if ((cpi->cpi_std[1].cp_edx & CPUID_INTC_EDX_HTT) != 0) {
		*ncpus = CPI_CPU_COUNT(cpi);
	} else {
		*ncpus = *ncores;
	}
}

static boolean_t
cpuid_leafB_getids(cpu_t *cpu)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	struct cpuid_regs regs;
	struct cpuid_regs *cp;

	if (cpi->cpi_maxeax < 0xB)
		return (B_FALSE);

	cp = &regs;
	cp->cp_eax = 0xB;
	cp->cp_edx = cp->cp_ebx = cp->cp_ecx = 0;

	(void) __cpuid_insn(cp);

	/*
	 * Check CPUID.EAX=0BH, ECX=0H:EBX is non-zero, which
	 * indicates that the extended topology enumeration leaf is
	 * available.
	 */
	if (cp->cp_ebx != 0) {
		uint32_t x2apic_id = 0;
		uint_t coreid_shift = 0;
		uint_t ncpu_per_core = 1;
		uint_t chipid_shift = 0;
		uint_t ncpu_per_chip = 1;
		uint_t i;
		uint_t level;

		for (i = 0; i < CPI_FNB_ECX_MAX; i++) {
			cp->cp_eax = 0xB;
			cp->cp_ecx = i;

			(void) __cpuid_insn(cp);
			level = CPI_CPU_LEVEL_TYPE(cp);

			if (level == 1) {
				x2apic_id = cp->cp_edx;
				coreid_shift = BITX(cp->cp_eax, 4, 0);
				ncpu_per_core = BITX(cp->cp_ebx, 15, 0);
			} else if (level == 2) {
				x2apic_id = cp->cp_edx;
				chipid_shift = BITX(cp->cp_eax, 4, 0);
				ncpu_per_chip = BITX(cp->cp_ebx, 15, 0);
			}
		}

		/*
		 * cpi_apicid is taken care of in cpuid_gather_apicid.
		 */
		cpi->cpi_ncpu_per_chip = ncpu_per_chip;
		cpi->cpi_ncore_per_chip = ncpu_per_chip /
		    ncpu_per_core;
		cpi->cpi_chipid = x2apic_id >> chipid_shift;
		cpi->cpi_clogid = x2apic_id & ((1 << chipid_shift) - 1);
		cpi->cpi_coreid = x2apic_id >> coreid_shift;
		cpi->cpi_pkgcoreid = cpi->cpi_clogid >> coreid_shift;
		cpi->cpi_procnodeid = cpi->cpi_chipid;
		cpi->cpi_compunitid = cpi->cpi_coreid;

		if (coreid_shift > 0 && chipid_shift > coreid_shift) {
			cpi->cpi_nthread_bits = coreid_shift;
			cpi->cpi_ncore_bits = chipid_shift - coreid_shift;
		}

		return (B_TRUE);
	} else {
		return (B_FALSE);
	}
}

static void
cpuid_intel_getids(cpu_t *cpu, void *feature)
{
	uint_t i;
	uint_t chipid_shift = 0;
	uint_t coreid_shift = 0;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	/*
	 * There are no compute units or processor nodes currently on Intel.
	 * Always set these to one.
	 */
	cpi->cpi_procnodes_per_pkg = 1;
	cpi->cpi_cores_per_compunit = 1;

	/*
	 * If cpuid Leaf B is present, use that to try and get this information.
	 * It will be the most accurate for Intel CPUs.
	 */
	if (cpuid_leafB_getids(cpu))
		return;

	/*
	 * In this case, we have the leaf 1 and leaf 4 values for ncpu_per_chip
	 * and ncore_per_chip. These represent the largest power of two values
	 * that we need to cover all of the IDs in the system. Therefore, we use
	 * those values to seed the number of bits needed to cover information
	 * in the case when leaf B is not available. These values will probably
	 * be larger than required, but that's OK.
	 */
	cpi->cpi_nthread_bits = ddi_fls(cpi->cpi_ncpu_per_chip);
	cpi->cpi_ncore_bits = ddi_fls(cpi->cpi_ncore_per_chip);

	for (i = 1; i < cpi->cpi_ncpu_per_chip; i <<= 1)
		chipid_shift++;

	cpi->cpi_chipid = cpi->cpi_apicid >> chipid_shift;
	cpi->cpi_clogid = cpi->cpi_apicid & ((1 << chipid_shift) - 1);

	if (is_x86_feature(feature, X86FSET_CMP)) {
		/*
		 * Multi-core (and possibly multi-threaded)
		 * processors.
		 */
		uint_t ncpu_per_core;
		if (cpi->cpi_ncore_per_chip == 1)
			ncpu_per_core = cpi->cpi_ncpu_per_chip;
		else if (cpi->cpi_ncore_per_chip > 1)
			ncpu_per_core = cpi->cpi_ncpu_per_chip /
			    cpi->cpi_ncore_per_chip;
		/*
		 * 8bit APIC IDs on dual core Pentiums
		 * look like this:
		 *
		 * +-----------------------+------+------+
		 * | Physical Package ID   |  MC  |  HT  |
		 * +-----------------------+------+------+
		 * <------- chipid -------->
		 * <------- coreid --------------->
		 *			   <--- clogid -->
		 *			   <------>
		 *			   pkgcoreid
		 *
		 * Where the number of bits necessary to
		 * represent MC and HT fields together equals
		 * to the minimum number of bits necessary to
		 * store the value of cpi->cpi_ncpu_per_chip.
		 * Of those bits, the MC part uses the number
		 * of bits necessary to store the value of
		 * cpi->cpi_ncore_per_chip.
		 */
		for (i = 1; i < ncpu_per_core; i <<= 1)
			coreid_shift++;
		cpi->cpi_coreid = cpi->cpi_apicid >> coreid_shift;
		cpi->cpi_pkgcoreid = cpi->cpi_clogid >> coreid_shift;
	} else if (is_x86_feature(feature, X86FSET_HTT)) {
		/*
		 * Single-core multi-threaded processors.
		 */
		cpi->cpi_coreid = cpi->cpi_chipid;
		cpi->cpi_pkgcoreid = 0;
	} else {
		/*
		 * Single-core single-thread processors.
		 */
		cpi->cpi_coreid = cpu->cpu_id;
		cpi->cpi_pkgcoreid = 0;
	}
	cpi->cpi_procnodeid = cpi->cpi_chipid;
	cpi->cpi_compunitid = cpi->cpi_coreid;
}

/*
 * Historically, AMD has had CMP chips with only a single thread per core.
 * However, starting in family 17h (Zen), this has changed and they now have
 * multiple threads. Our internal core id needs to be a unique value.
 *
 * To determine the core id of an AMD system, if we're from a family before 17h,
 * then we just use the cpu id, as that gives us a good value that will be
 * unique for each core. If instead, we're on family 17h or later, then we need
 * to do something more complicated. CPUID leaf 0x8000001e can tell us
 * how many threads are in the system. Based on that, we'll shift the APIC ID.
 * We can't use the normal core id in that leaf as it's only unique within the
 * socket, which is perfect for cpi_pkgcoreid, but not us.
 */
static id_t
cpuid_amd_get_coreid(cpu_t *cpu)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	if (cpi->cpi_family >= 0x17 &&
	    is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e) {
		uint_t nthreads = BITX(cpi->cpi_extd[0x1e].cp_ebx, 15, 8) + 1;
		if (nthreads > 1) {
			VERIFY3U(nthreads, ==, 2);
			return (cpi->cpi_apicid >> 1);
		}
	}

	return (cpu->cpu_id);
}

/*
 * IDs on AMD is a more challenging task. This is notable because of the
 * following two facts:
 *
 *  1. Before family 0x17 (Zen), there was no support for SMT and there was
 *     also no way to get an actual unique core id from the system. As such, we
 *     synthesize this case by using cpu->cpu_id.  This scheme does not,
 *     however, guarantee that sibling cores of a chip will have sequential
 *     coreids starting at a multiple of the number of cores per chip - that is
 *     usually the case, but if the ACPI MADT table is presented in a different
 *     order then we need to perform a few more gymnastics for the pkgcoreid.
 *
 *  2. In families 0x15 and 16x (Bulldozer and co.) the cores came in groups
 *     called compute units. These compute units share the L1I cache, L2 cache,
 *     and the FPU. To deal with this, a new topology leaf was added in
 *     0x8000001e. However, parts of this leaf have different meanings
 *     once we get to family 0x17.
 */

static void
cpuid_amd_getids(cpu_t *cpu, uchar_t *features)
{
	int i, first_half, coreidsz;
	uint32_t nb_caps_reg;
	uint_t node2_1;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	struct cpuid_regs *cp;

	/*
	 * Calculate the core id (this comes from hardware in family 0x17 if it
	 * hasn't been stripped by virtualization). We always set the compute
	 * unit id to the same value. Also, initialize the default number of
	 * cores per compute unit and nodes per package. This will be
	 * overwritten when we know information about a particular family.
	 */
	cpi->cpi_coreid = cpuid_amd_get_coreid(cpu);
	cpi->cpi_compunitid = cpi->cpi_coreid;
	cpi->cpi_cores_per_compunit = 1;
	cpi->cpi_procnodes_per_pkg = 1;

	/*
	 * To construct the logical ID, we need to determine how many APIC IDs
	 * are dedicated to the cores and threads. This is provided for us in
	 * 0x80000008. However, if it's not present (say due to virtualization),
	 * then we assume it's one. This should be present on all 64-bit AMD
	 * processors.  It was added in family 0xf (Hammer).
	 */
	if (cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_8) {
		coreidsz = BITX((cpi)->cpi_extd[8].cp_ecx, 15, 12);

		/*
		 * In AMD parlance chip is really a node while illumos
		 * uses chip as equivalent to socket/package.
		 */
		if (coreidsz == 0) {
			/* Use legacy method */
			for (i = 1; i < cpi->cpi_ncore_per_chip; i <<= 1)
				coreidsz++;
			if (coreidsz == 0)
				coreidsz = 1;
		}
	} else {
		/* Assume single-core part */
		coreidsz = 1;
	}
	cpi->cpi_clogid = cpi->cpi_apicid & ((1 << coreidsz) - 1);

	/*
	 * The package core ID varies depending on the family. While it may be
	 * tempting to use the CPUID_LEAF_EXT_1e %ebx core id, unfortunately,
	 * this value is the core id in the given node. For non-virtualized
	 * family 17h, we need to take the logical core id and shift off the
	 * threads like we do when getting the core id.  Otherwise, we can use
	 * the clogid as is. When family 17h is virtualized, the clogid should
	 * be sufficient as if we don't have valid data in the leaf, then we
	 * won't think we have SMT, in which case the cpi_clogid should be
	 * sufficient.
	 */
	if (cpi->cpi_family >= 0x17 &&
	    is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e &&
	    cpi->cpi_extd[0x1e].cp_ebx != 0) {
		uint_t nthreads = BITX(cpi->cpi_extd[0x1e].cp_ebx, 15, 8) + 1;
		if (nthreads > 1) {
			VERIFY3U(nthreads, ==, 2);
			cpi->cpi_pkgcoreid = cpi->cpi_clogid >> 1;
		} else {
			cpi->cpi_pkgcoreid = cpi->cpi_clogid;
		}
	} else {
		cpi->cpi_pkgcoreid = cpi->cpi_clogid;
	}

	/*
	 * Obtain the node ID and compute unit IDs. If we're on family 0x15
	 * (bulldozer) or newer, then we can derive all of this from leaf
	 * CPUID_LEAF_EXT_1e. Otherwise, the method varies by family.
	 */
	if (is_x86_feature(x86_featureset, X86FSET_TOPOEXT) &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1e) {
		cp = &cpi->cpi_extd[0x1e];

		cpi->cpi_procnodes_per_pkg = BITX(cp->cp_ecx, 10, 8) + 1;
		cpi->cpi_procnodeid = BITX(cp->cp_ecx, 7, 0);

		/*
		 * For Bulldozer-era CPUs, recalculate the compute unit
		 * information.
		 */
		if (cpi->cpi_family >= 0x15 && cpi->cpi_family < 0x17) {
			cpi->cpi_cores_per_compunit =
			    BITX(cp->cp_ebx, 15, 8) + 1;
			cpi->cpi_compunitid = BITX(cp->cp_ebx, 7, 0) +
			    (cpi->cpi_ncore_per_chip /
			    cpi->cpi_cores_per_compunit) *
			    (cpi->cpi_procnodeid /
			    cpi->cpi_procnodes_per_pkg);
		}
	} else if (cpi->cpi_family == 0xf || cpi->cpi_family >= 0x11) {
		cpi->cpi_procnodeid = (cpi->cpi_apicid >> coreidsz) & 7;
	} else if (cpi->cpi_family == 0x10) {
		/*
		 * See if we are a multi-node processor.
		 * All processors in the system have the same number of nodes
		 */
		nb_caps_reg =  pci_getl_func(0, 24, 3, 0xe8);
		if ((cpi->cpi_model < 8) || BITX(nb_caps_reg, 29, 29) == 0) {
			/* Single-node */
			cpi->cpi_procnodeid = BITX(cpi->cpi_apicid, 5,
			    coreidsz);
		} else {

			/*
			 * Multi-node revision D (2 nodes per package
			 * are supported)
			 */
			cpi->cpi_procnodes_per_pkg = 2;

			first_half = (cpi->cpi_pkgcoreid <=
			    (cpi->cpi_ncore_per_chip/2 - 1));

			if (cpi->cpi_apicid == cpi->cpi_pkgcoreid) {
				/* We are BSP */
				cpi->cpi_procnodeid = (first_half ? 0 : 1);
			} else {

				/* We are AP */
				/* NodeId[2:1] bits to use for reading F3xe8 */
				node2_1 = BITX(cpi->cpi_apicid, 5, 4) << 1;

				nb_caps_reg =
				    pci_getl_func(0, 24 + node2_1, 3, 0xe8);

				/*
				 * Check IntNodeNum bit (31:30, but bit 31 is
				 * always 0 on dual-node processors)
				 */
				if (BITX(nb_caps_reg, 30, 30) == 0)
					cpi->cpi_procnodeid = node2_1 +
					    !first_half;
				else
					cpi->cpi_procnodeid = node2_1 +
					    first_half;
			}
		}
	} else {
		cpi->cpi_procnodeid = 0;
	}

	cpi->cpi_chipid =
	    cpi->cpi_procnodeid / cpi->cpi_procnodes_per_pkg;

	cpi->cpi_ncore_bits = coreidsz;
	cpi->cpi_nthread_bits = ddi_fls(cpi->cpi_ncpu_per_chip /
	    cpi->cpi_ncore_per_chip);
}

static void
spec_uarch_flush_noop(void)
{
}

/*
 * When microcode is present that mitigates MDS, this wrmsr will also flush the
 * MDS-related micro-architectural state that would normally happen by calling
 * x86_md_clear().
 */
static void
spec_uarch_flush_msr(void)
{
	wrmsr(MSR_IA32_FLUSH_CMD, IA32_FLUSH_CMD_L1D);
}

/*
 * This function points to a function that will flush certain
 * micro-architectural state on the processor. This flush is used to mitigate
 * two different classes of Intel CPU vulnerabilities: L1TF and MDS. This
 * function can point to one of three functions:
 *
 * - A noop which is done because we either are vulnerable, but do not have
 *   microcode available to help deal with a fix, or because we aren't
 *   vulnerable.
 *
 * - spec_uarch_flush_msr which will issue an L1D flush and if microcode to
 *   mitigate MDS is present, also perform the equivalent of the MDS flush;
 *   however, it only flushes the MDS related micro-architectural state on the
 *   current hyperthread, it does not do anything for the twin.
 *
 * - x86_md_clear which will flush the MDS related state. This is done when we
 *   have a processor that is vulnerable to MDS, but is not vulnerable to L1TF
 *   (RDCL_NO is set).
 */
void (*spec_uarch_flush)(void) = spec_uarch_flush_noop;

static void
cpuid_update_md_clear(cpu_t *cpu, uchar_t *featureset)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	/*
	 * While RDCL_NO indicates that one of the MDS vulnerabilities (MSBDS)
	 * has been fixed in hardware, it doesn't cover everything related to
	 * MDS. Therefore we can only rely on MDS_NO to determine that we don't
	 * need to mitigate this.
	 */
	if (cpi->cpi_vendor != X86_VENDOR_Intel ||
	    is_x86_feature(featureset, X86FSET_MDS_NO)) {
		return;
	}

	if (is_x86_feature(featureset, X86FSET_MD_CLEAR)) {
		const uint8_t nop = NOP_INSTR;
		uint8_t *md = (uint8_t *)x86_md_clear;

		*md = nop;
	}

	membar_producer();
}

static void
cpuid_update_l1d_flush(cpu_t *cpu, uchar_t *featureset)
{
	boolean_t need_l1d, need_mds;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	/*
	 * If we're not on Intel or we've mitigated both RDCL and MDS in
	 * hardware, then there's nothing left for us to do for enabling the
	 * flush. We can also go ahead and say that SMT exclusion is
	 * unnecessary.
	 */
	if (cpi->cpi_vendor != X86_VENDOR_Intel ||
	    (is_x86_feature(featureset, X86FSET_RDCL_NO) &&
	    is_x86_feature(featureset, X86FSET_MDS_NO))) {
		extern int smt_exclusion;
		smt_exclusion = 0;
		spec_uarch_flush = spec_uarch_flush_noop;
		membar_producer();
		return;
	}

	/*
	 * The locations where we need to perform an L1D flush are required both
	 * for mitigating L1TF and MDS. When verw support is present in
	 * microcode, then the L1D flush will take care of doing that as well.
	 * However, if we have a system where RDCL_NO is present, but we don't
	 * have MDS_NO, then we need to do a verw (x86_md_clear) and not a full
	 * L1D flush.
	 */
	if (!is_x86_feature(featureset, X86FSET_RDCL_NO) &&
	    is_x86_feature(featureset, X86FSET_FLUSH_CMD) &&
	    !is_x86_feature(featureset, X86FSET_L1D_VM_NO)) {
		need_l1d = B_TRUE;
	} else {
		need_l1d = B_FALSE;
	}

	if (!is_x86_feature(featureset, X86FSET_MDS_NO) &&
	    is_x86_feature(featureset, X86FSET_MD_CLEAR)) {
		need_mds = B_TRUE;
	} else {
		need_mds = B_FALSE;
	}

	if (need_l1d) {
		spec_uarch_flush = spec_uarch_flush_msr;
	} else if (need_mds) {
		spec_uarch_flush = x86_md_clear;
	} else {
		/*
		 * We have no hardware mitigations available to us.
		 */
		spec_uarch_flush = spec_uarch_flush_noop;
	}
	membar_producer();
}

/*
 * We default to enabling RSB mitigations.
 */
static void
cpuid_patch_rsb(x86_spectrev2_mitigation_t mit)
{
	const uint8_t ret = RET_INSTR;
	uint8_t *stuff = (uint8_t *)x86_rsb_stuff;

	switch (mit) {
	case X86_SPECTREV2_ENHANCED_IBRS:
	case X86_SPECTREV2_DISABLED:
		*stuff = ret;
		break;
	default:
		break;
	}
}

static void
cpuid_patch_retpolines(x86_spectrev2_mitigation_t mit)
{
	const char *thunks[] = { "_rax", "_rbx", "_rcx", "_rdx", "_rdi",
	    "_rsi", "_rbp", "_r8", "_r9", "_r10", "_r11", "_r12", "_r13",
	    "_r14", "_r15" };
	const uint_t nthunks = ARRAY_SIZE(thunks);
	const char *type;
	uint_t i;

	if (mit == x86_spectrev2_mitigation)
		return;

	switch (mit) {
	case X86_SPECTREV2_RETPOLINE:
		type = "gen";
		break;
	case X86_SPECTREV2_RETPOLINE_AMD:
		type = "amd";
		break;
	case X86_SPECTREV2_ENHANCED_IBRS:
	case X86_SPECTREV2_DISABLED:
		type = "jmp";
		break;
	default:
		panic("asked to updated retpoline state with unknown state!");
	}

	for (i = 0; i < nthunks; i++) {
		uintptr_t source, dest;
		int ssize, dsize;
		char sourcebuf[64], destbuf[64];
		size_t len;

		(void) snprintf(destbuf, sizeof (destbuf),
		    "__x86_indirect_thunk%s", thunks[i]);
		(void) snprintf(sourcebuf, sizeof (sourcebuf),
		    "__x86_indirect_thunk_%s%s", type, thunks[i]);

		source = kobj_getelfsym(sourcebuf, NULL, &ssize);
		dest = kobj_getelfsym(destbuf, NULL, &dsize);
		VERIFY3U(source, !=, 0);
		VERIFY3U(dest, !=, 0);
		VERIFY3S(dsize, >=, ssize);
		bcopy((void *)source, (void *)dest, ssize);
	}
}

static void
cpuid_enable_enhanced_ibrs(void)
{
	uint64_t val;

	val = rdmsr(MSR_IA32_SPEC_CTRL);
	val |= IA32_SPEC_CTRL_IBRS;
	wrmsr(MSR_IA32_SPEC_CTRL, val);
}

#ifndef __xpv
/*
 * Determine whether or not we can use the AMD optimized retpoline
 * functionality. We use this when we know we're on an AMD system and we can
 * successfully verify that lfence is dispatch serializing.
 */
static boolean_t
cpuid_use_amd_retpoline(struct cpuid_info *cpi)
{
	uint64_t val;
	on_trap_data_t otd;

	if (cpi->cpi_vendor != X86_VENDOR_AMD)
		return (B_FALSE);

	/*
	 * We need to determine whether or not lfence is serializing. It always
	 * is on families 0xf and 0x11. On others, it's controlled by
	 * MSR_AMD_DECODE_CONFIG (MSRC001_1029). If some hypervisor gives us a
	 * crazy old family, don't try and do anything.
	 */
	if (cpi->cpi_family < 0xf)
		return (B_FALSE);
	if (cpi->cpi_family == 0xf || cpi->cpi_family == 0x11)
		return (B_TRUE);

	/*
	 * While it may be tempting to use get_hwenv(), there are no promises
	 * that a hypervisor will actually declare themselves to be so in a
	 * friendly way. As such, try to read and set the MSR. If we can then
	 * read back the value we set (it wasn't just set to zero), then we go
	 * for it.
	 */
	if (!on_trap(&otd, OT_DATA_ACCESS)) {
		val = rdmsr(MSR_AMD_DECODE_CONFIG);
		val |= AMD_DECODE_CONFIG_LFENCE_DISPATCH;
		wrmsr(MSR_AMD_DECODE_CONFIG, val);
		val = rdmsr(MSR_AMD_DECODE_CONFIG);
	} else {
		val = 0;
	}
	no_trap();

	if ((val & AMD_DECODE_CONFIG_LFENCE_DISPATCH) != 0)
		return (B_TRUE);
	return (B_FALSE);
}
#endif	/* !__xpv */

static void
cpuid_scan_security(cpu_t *cpu, uchar_t *featureset)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	x86_spectrev2_mitigation_t v2mit;

	if (cpi->cpi_vendor == X86_VENDOR_AMD &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_8) {
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_IBPB)
			add_x86_feature(featureset, X86FSET_IBPB);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_IBRS)
			add_x86_feature(featureset, X86FSET_IBRS);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_STIBP)
			add_x86_feature(featureset, X86FSET_STIBP);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_STIBP_ALL)
			add_x86_feature(featureset, X86FSET_STIBP_ALL);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_SSBD)
			add_x86_feature(featureset, X86FSET_SSBD);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_VIRT_SSBD)
			add_x86_feature(featureset, X86FSET_SSBD_VIRT);
		if (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_SSB_NO)
			add_x86_feature(featureset, X86FSET_SSB_NO);
		/*
		 * Don't enable enhanced IBRS unless we're told that we should
		 * prefer it and it has the same semantics as Intel. This is
		 * split into two bits rather than a single one.
		 */
		if ((cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_PREFER_IBRS) &&
		    (cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_IBRS_ALL)) {
			add_x86_feature(featureset, X86FSET_IBRS_ALL);
		}

	} else if (cpi->cpi_vendor == X86_VENDOR_Intel &&
	    cpi->cpi_maxeax >= 7) {
		struct cpuid_regs *ecp;
		ecp = &cpi->cpi_std[7];

		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_MD_CLEAR) {
			add_x86_feature(featureset, X86FSET_MD_CLEAR);
		}

		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_SPEC_CTRL) {
			add_x86_feature(featureset, X86FSET_IBRS);
			add_x86_feature(featureset, X86FSET_IBPB);
		}

		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_STIBP) {
			add_x86_feature(featureset, X86FSET_STIBP);
		}

		/*
		 * Don't read the arch caps MSR on xpv where we lack the
		 * on_trap().
		 */
#ifndef __xpv
		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_ARCH_CAPS) {
			on_trap_data_t otd;

			/*
			 * Be paranoid and assume we'll get a #GP.
			 */
			if (!on_trap(&otd, OT_DATA_ACCESS)) {
				uint64_t reg;

				reg = rdmsr(MSR_IA32_ARCH_CAPABILITIES);
				if (reg & IA32_ARCH_CAP_RDCL_NO) {
					add_x86_feature(featureset,
					    X86FSET_RDCL_NO);
				}
				if (reg & IA32_ARCH_CAP_IBRS_ALL) {
					add_x86_feature(featureset,
					    X86FSET_IBRS_ALL);
				}
				if (reg & IA32_ARCH_CAP_RSBA) {
					add_x86_feature(featureset,
					    X86FSET_RSBA);
				}
				if (reg & IA32_ARCH_CAP_SKIP_L1DFL_VMENTRY) {
					add_x86_feature(featureset,
					    X86FSET_L1D_VM_NO);
				}
				if (reg & IA32_ARCH_CAP_SSB_NO) {
					add_x86_feature(featureset,
					    X86FSET_SSB_NO);
				}
				if (reg & IA32_ARCH_CAP_MDS_NO) {
					add_x86_feature(featureset,
					    X86FSET_MDS_NO);
				}
			}
			no_trap();
		}
#endif	/* !__xpv */

		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_SSBD)
			add_x86_feature(featureset, X86FSET_SSBD);

		if (ecp->cp_edx & CPUID_INTC_EDX_7_0_FLUSH_CMD)
			add_x86_feature(featureset, X86FSET_FLUSH_CMD);
	}

	if (cpu->cpu_id != 0) {
		if (x86_spectrev2_mitigation == X86_SPECTREV2_ENHANCED_IBRS) {
			cpuid_enable_enhanced_ibrs();
		}
		return;
	}

	/*
	 * Go through and initialize various security mechanisms that we should
	 * only do on a single CPU. This includes Spectre V2, L1TF, and MDS.
	 */

	/*
	 * By default we've come in with retpolines enabled. Check whether we
	 * should disable them or enable enhanced IBRS. RSB stuffing is enabled
	 * by default, but disabled if we are using enhanced IBRS.
	 */
	if (x86_disable_spectrev2 != 0) {
		v2mit = X86_SPECTREV2_DISABLED;
	} else if (is_x86_feature(featureset, X86FSET_IBRS_ALL)) {
		cpuid_enable_enhanced_ibrs();
		v2mit = X86_SPECTREV2_ENHANCED_IBRS;
#ifndef __xpv
	} else if (cpuid_use_amd_retpoline(cpi)) {
		v2mit = X86_SPECTREV2_RETPOLINE_AMD;
#endif	/* !__xpv */
	} else {
		v2mit = X86_SPECTREV2_RETPOLINE;
	}

	cpuid_patch_retpolines(v2mit);
	cpuid_patch_rsb(v2mit);
	x86_spectrev2_mitigation = v2mit;
	membar_producer();

	/*
	 * We need to determine what changes are required for mitigating L1TF
	 * and MDS. If the CPU suffers from either of them, then SMT exclusion
	 * is required.
	 *
	 * If any of these are present, then we need to flush u-arch state at
	 * various points. For MDS, we need to do so whenever we change to a
	 * lesser privilege level or we are halting the CPU. For L1TF we need to
	 * flush the L1D cache at VM entry. When we have microcode that handles
	 * MDS, the L1D flush also clears the other u-arch state that the
	 * md_clear does.
	 */

	/*
	 * Update whether or not we need to be taking explicit action against
	 * MDS.
	 */
	cpuid_update_md_clear(cpu, featureset);

	/*
	 * Determine whether SMT exclusion is required and whether or not we
	 * need to perform an l1d flush.
	 */
	cpuid_update_l1d_flush(cpu, featureset);
}

/*
 * Setup XFeature_Enabled_Mask register. Required by xsave feature.
 */
void
setup_xfem(void)
{
	uint64_t flags = XFEATURE_LEGACY_FP;

	ASSERT(is_x86_feature(x86_featureset, X86FSET_XSAVE));

	if (is_x86_feature(x86_featureset, X86FSET_SSE))
		flags |= XFEATURE_SSE;

	if (is_x86_feature(x86_featureset, X86FSET_AVX))
		flags |= XFEATURE_AVX;

	if (is_x86_feature(x86_featureset, X86FSET_AVX512F))
		flags |= XFEATURE_AVX512;

	set_xcr(XFEATURE_ENABLED_MASK, flags);

	xsave_bv_all = flags;
}

static void
cpuid_pass1_topology(cpu_t *cpu, uchar_t *featureset)
{
	struct cpuid_info *cpi;

	cpi = cpu->cpu_m.mcpu_cpi;

	if (cpi->cpi_vendor == X86_VENDOR_AMD) {
		cpuid_gather_amd_topology_leaves(cpu);
	}

	cpi->cpi_apicid = cpuid_gather_apicid(cpi);

	/*
	 * Before we can calculate the IDs that we should assign to this
	 * processor, we need to understand how many cores and threads it has.
	 */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		cpuid_intel_ncores(cpi, &cpi->cpi_ncpu_per_chip,
		    &cpi->cpi_ncore_per_chip);
		break;
	case X86_VENDOR_AMD:
		cpuid_amd_ncores(cpi, &cpi->cpi_ncpu_per_chip,
		    &cpi->cpi_ncore_per_chip);
		break;
	default:
		/*
		 * If we have some other x86 compatible chip, it's not clear how
		 * they would behave. The most common case is virtualization
		 * today, though there are also 64-bit VIA chips. Assume that
		 * all we can get is the basic Leaf 1 HTT information.
		 */
		if ((cpi->cpi_std[1].cp_edx & CPUID_INTC_EDX_HTT) != 0) {
			cpi->cpi_ncore_per_chip = 1;
			cpi->cpi_ncpu_per_chip = CPI_CPU_COUNT(cpi);
		}
		break;
	}

	/*
	 * Based on the calculated number of threads and cores, potentially
	 * assign the HTT and CMT features.
	 */
	if (cpi->cpi_ncore_per_chip > 1) {
		add_x86_feature(featureset, X86FSET_CMP);
	}

	if (cpi->cpi_ncpu_per_chip > 1 &&
	    cpi->cpi_ncpu_per_chip != cpi->cpi_ncore_per_chip) {
		add_x86_feature(featureset, X86FSET_HTT);
	}

	/*
	 * Now that has been set up, we need to go through and calculate all of
	 * the rest of the parameters that exist. If we think the CPU doesn't
	 * have either SMT (HTT) or CMP, then we basically go through and fake
	 * up information in some way. The most likely case for this is
	 * virtualization where we have a lot of partial topology information.
	 */
	if (!is_x86_feature(featureset, X86FSET_HTT) &&
	    !is_x86_feature(featureset, X86FSET_CMP)) {
		/*
		 * This is a single core, single-threaded processor.
		 */
		cpi->cpi_procnodes_per_pkg = 1;
		cpi->cpi_cores_per_compunit = 1;
		cpi->cpi_compunitid = 0;
		cpi->cpi_chipid = -1;
		cpi->cpi_clogid = 0;
		cpi->cpi_coreid = cpu->cpu_id;
		cpi->cpi_pkgcoreid = 0;
		if (cpi->cpi_vendor == X86_VENDOR_AMD) {
			cpi->cpi_procnodeid = BITX(cpi->cpi_apicid, 3, 0);
		} else {
			cpi->cpi_procnodeid = cpi->cpi_chipid;
		}
	} else {
		switch (cpi->cpi_vendor) {
		case X86_VENDOR_Intel:
			cpuid_intel_getids(cpu, featureset);
			break;
		case X86_VENDOR_AMD:
			cpuid_amd_getids(cpu, featureset);
			break;
		default:
			/*
			 * In this case, it's hard to say what we should do.
			 * We're going to model them to the OS as single core
			 * threads. We don't have a good identifier for them, so
			 * we're just going to use the cpu id all on a single
			 * chip.
			 *
			 * This case has historically been different from the
			 * case above where we don't have HTT or CMP. While they
			 * could be combined, we've opted to keep it separate to
			 * minimize the risk of topology changes in weird cases.
			 */
			cpi->cpi_procnodes_per_pkg = 1;
			cpi->cpi_cores_per_compunit = 1;
			cpi->cpi_chipid = 0;
			cpi->cpi_coreid = cpu->cpu_id;
			cpi->cpi_clogid = cpu->cpu_id;
			cpi->cpi_pkgcoreid = cpu->cpu_id;
			cpi->cpi_procnodeid = cpi->cpi_chipid;
			cpi->cpi_compunitid = cpi->cpi_coreid;
			break;
		}
	}
}

/*
 * Gather relevant CPU features from leaf 6 which covers thermal information. We
 * always gather leaf 6 if it's supported; however, we only look for features on
 * Intel systems as AMD does not currently define any of the features we look
 * for below.
 */
static void
cpuid_pass1_thermal(cpu_t *cpu, uchar_t *featureset)
{
	struct cpuid_regs *cp;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	if (cpi->cpi_maxeax < 6) {
		return;
	}

	cp = &cpi->cpi_std[6];
	cp->cp_eax = 6;
	cp->cp_ebx = cp->cp_ecx = cp->cp_edx = 0;
	(void) __cpuid_insn(cp);
	platform_cpuid_mangle(cpi->cpi_vendor, 6, cp);

	if (cpi->cpi_vendor != X86_VENDOR_Intel) {
		return;
	}

	if ((cp->cp_eax & CPUID_INTC_EAX_DTS) != 0) {
		add_x86_feature(featureset, X86FSET_CORE_THERMAL);
	}

	if ((cp->cp_eax & CPUID_INTC_EAX_PTM) != 0) {
		add_x86_feature(featureset, X86FSET_PKG_THERMAL);
	}
}

void
cpuid_pass1(cpu_t *cpu, uchar_t *featureset)
{
	uint32_t mask_ecx, mask_edx;
	struct cpuid_info *cpi;
	struct cpuid_regs *cp;
	int xcpuid;
#if !defined(__xpv)
	extern int idle_cpu_prefer_mwait;
#endif

	/*
	 * Space statically allocated for BSP, ensure pointer is set
	 */
	if (cpu->cpu_id == 0) {
		if (cpu->cpu_m.mcpu_cpi == NULL)
			cpu->cpu_m.mcpu_cpi = &cpuid_info0;
	}

	add_x86_feature(featureset, X86FSET_CPUID);

	cpi = cpu->cpu_m.mcpu_cpi;
	ASSERT(cpi != NULL);
	cp = &cpi->cpi_std[0];
	cp->cp_eax = 0;
	cpi->cpi_maxeax = __cpuid_insn(cp);
	{
		uint32_t *iptr = (uint32_t *)cpi->cpi_vendorstr;
		*iptr++ = cp->cp_ebx;
		*iptr++ = cp->cp_edx;
		*iptr++ = cp->cp_ecx;
		*(char *)&cpi->cpi_vendorstr[12] = '\0';
	}

	cpi->cpi_vendor = _cpuid_vendorstr_to_vendorcode(cpi->cpi_vendorstr);
	x86_vendor = cpi->cpi_vendor; /* for compatibility */

	/*
	 * Limit the range in case of weird hardware
	 */
	if (cpi->cpi_maxeax > CPI_MAXEAX_MAX)
		cpi->cpi_maxeax = CPI_MAXEAX_MAX;
	if (cpi->cpi_maxeax < 1)
		goto pass1_done;

	cp = &cpi->cpi_std[1];
	cp->cp_eax = 1;
	(void) __cpuid_insn(cp);

	/*
	 * Extract identifying constants for easy access.
	 */
	cpi->cpi_model = CPI_MODEL(cpi);
	cpi->cpi_family = CPI_FAMILY(cpi);

	if (cpi->cpi_family == 0xf)
		cpi->cpi_family += CPI_FAMILY_XTD(cpi);

	/*
	 * Beware: AMD uses "extended model" iff base *FAMILY* == 0xf.
	 * Intel, and presumably everyone else, uses model == 0xf, as
	 * one would expect (max value means possible overflow).  Sigh.
	 */

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		if (IS_EXTENDED_MODEL_INTEL(cpi))
			cpi->cpi_model += CPI_MODEL_XTD(cpi) << 4;
		break;
	case X86_VENDOR_AMD:
		if (CPI_FAMILY(cpi) == 0xf)
			cpi->cpi_model += CPI_MODEL_XTD(cpi) << 4;
		break;
	default:
		if (cpi->cpi_model == 0xf)
			cpi->cpi_model += CPI_MODEL_XTD(cpi) << 4;
		break;
	}

	cpi->cpi_step = CPI_STEP(cpi);
	cpi->cpi_brandid = CPI_BRANDID(cpi);

	/*
	 * *default* assumptions:
	 * - believe %edx feature word
	 * - ignore %ecx feature word
	 * - 32-bit virtual and physical addressing
	 */
	mask_edx = 0xffffffff;
	mask_ecx = 0;

	cpi->cpi_pabits = cpi->cpi_vabits = 32;

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		if (cpi->cpi_family == 5)
			x86_type = X86_TYPE_P5;
		else if (IS_LEGACY_P6(cpi)) {
			x86_type = X86_TYPE_P6;
			pentiumpro_bug4046376 = 1;
			/*
			 * Clear the SEP bit when it was set erroneously
			 */
			if (cpi->cpi_model < 3 && cpi->cpi_step < 3)
				cp->cp_edx &= ~CPUID_INTC_EDX_SEP;
		} else if (IS_NEW_F6(cpi) || cpi->cpi_family == 0xf) {
			x86_type = X86_TYPE_P4;
			/*
			 * We don't currently depend on any of the %ecx
			 * features until Prescott, so we'll only check
			 * this from P4 onwards.  We might want to revisit
			 * that idea later.
			 */
			mask_ecx = 0xffffffff;
		} else if (cpi->cpi_family > 0xf)
			mask_ecx = 0xffffffff;
		/*
		 * We don't support MONITOR/MWAIT if leaf 5 is not available
		 * to obtain the monitor linesize.
		 */
		if (cpi->cpi_maxeax < 5)
			mask_ecx &= ~CPUID_INTC_ECX_MON;
		break;
	case X86_VENDOR_IntelClone:
	default:
		break;
	case X86_VENDOR_AMD:
#if defined(OPTERON_ERRATUM_108)
		if (cpi->cpi_family == 0xf && cpi->cpi_model == 0xe) {
			cp->cp_eax = (0xf0f & cp->cp_eax) | 0xc0;
			cpi->cpi_model = 0xc;
		} else
#endif
		if (cpi->cpi_family == 5) {
			/*
			 * AMD K5 and K6
			 *
			 * These CPUs have an incomplete implementation
			 * of MCA/MCE which we mask away.
			 */
			mask_edx &= ~(CPUID_INTC_EDX_MCE | CPUID_INTC_EDX_MCA);

			/*
			 * Model 0 uses the wrong (APIC) bit
			 * to indicate PGE.  Fix it here.
			 */
			if (cpi->cpi_model == 0) {
				if (cp->cp_edx & 0x200) {
					cp->cp_edx &= ~0x200;
					cp->cp_edx |= CPUID_INTC_EDX_PGE;
				}
			}

			/*
			 * Early models had problems w/ MMX; disable.
			 */
			if (cpi->cpi_model < 6)
				mask_edx &= ~CPUID_INTC_EDX_MMX;
		}

		/*
		 * For newer families, SSE3 and CX16, at least, are valid;
		 * enable all
		 */
		if (cpi->cpi_family >= 0xf)
			mask_ecx = 0xffffffff;
		/*
		 * We don't support MONITOR/MWAIT if leaf 5 is not available
		 * to obtain the monitor linesize.
		 */
		if (cpi->cpi_maxeax < 5)
			mask_ecx &= ~CPUID_INTC_ECX_MON;

#if !defined(__xpv)
		/*
		 * AMD has not historically used MWAIT in the CPU's idle loop.
		 * Pre-family-10h Opterons do not have the MWAIT instruction. We
		 * know for certain that in at least family 17h, per AMD, mwait
		 * is preferred. Families in-between are less certain.
		 */
		if (cpi->cpi_family < 0x17) {
			idle_cpu_prefer_mwait = 0;
		}
#endif

		break;
	case X86_VENDOR_TM:
		/*
		 * workaround the NT workaround in CMS 4.1
		 */
		if (cpi->cpi_family == 5 && cpi->cpi_model == 4 &&
		    (cpi->cpi_step == 2 || cpi->cpi_step == 3))
			cp->cp_edx |= CPUID_INTC_EDX_CX8;
		break;
	case X86_VENDOR_Centaur:
		/*
		 * workaround the NT workarounds again
		 */
		if (cpi->cpi_family == 6)
			cp->cp_edx |= CPUID_INTC_EDX_CX8;
		break;
	case X86_VENDOR_Cyrix:
		/*
		 * We rely heavily on the probing in locore
		 * to actually figure out what parts, if any,
		 * of the Cyrix cpuid instruction to believe.
		 */
		switch (x86_type) {
		case X86_TYPE_CYRIX_486:
			mask_edx = 0;
			break;
		case X86_TYPE_CYRIX_6x86:
			mask_edx = 0;
			break;
		case X86_TYPE_CYRIX_6x86L:
			mask_edx =
			    CPUID_INTC_EDX_DE |
			    CPUID_INTC_EDX_CX8;
			break;
		case X86_TYPE_CYRIX_6x86MX:
			mask_edx =
			    CPUID_INTC_EDX_DE |
			    CPUID_INTC_EDX_MSR |
			    CPUID_INTC_EDX_CX8 |
			    CPUID_INTC_EDX_PGE |
			    CPUID_INTC_EDX_CMOV |
			    CPUID_INTC_EDX_MMX;
			break;
		case X86_TYPE_CYRIX_GXm:
			mask_edx =
			    CPUID_INTC_EDX_MSR |
			    CPUID_INTC_EDX_CX8 |
			    CPUID_INTC_EDX_CMOV |
			    CPUID_INTC_EDX_MMX;
			break;
		case X86_TYPE_CYRIX_MediaGX:
			break;
		case X86_TYPE_CYRIX_MII:
		case X86_TYPE_VIA_CYRIX_III:
			mask_edx =
			    CPUID_INTC_EDX_DE |
			    CPUID_INTC_EDX_TSC |
			    CPUID_INTC_EDX_MSR |
			    CPUID_INTC_EDX_CX8 |
			    CPUID_INTC_EDX_PGE |
			    CPUID_INTC_EDX_CMOV |
			    CPUID_INTC_EDX_MMX;
			break;
		default:
			break;
		}
		break;
	}

#if defined(__xpv)
	/*
	 * Do not support MONITOR/MWAIT under a hypervisor
	 */
	mask_ecx &= ~CPUID_INTC_ECX_MON;
	/*
	 * Do not support XSAVE under a hypervisor for now
	 */
	xsave_force_disable = B_TRUE;

#endif	/* __xpv */

	if (xsave_force_disable) {
		mask_ecx &= ~CPUID_INTC_ECX_XSAVE;
		mask_ecx &= ~CPUID_INTC_ECX_AVX;
		mask_ecx &= ~CPUID_INTC_ECX_F16C;
		mask_ecx &= ~CPUID_INTC_ECX_FMA;
	}

	/*
	 * Now we've figured out the masks that determine
	 * which bits we choose to believe, apply the masks
	 * to the feature words, then map the kernel's view
	 * of these feature words into its feature word.
	 */
	cp->cp_edx &= mask_edx;
	cp->cp_ecx &= mask_ecx;

	/*
	 * apply any platform restrictions (we don't call this
	 * immediately after __cpuid_insn here, because we need the
	 * workarounds applied above first)
	 */
	platform_cpuid_mangle(cpi->cpi_vendor, 1, cp);

	/*
	 * In addition to ecx and edx, Intel and AMD are storing a bunch of
	 * instruction set extensions in leaf 7's ebx, ecx, and edx.
	 */
	if (cpi->cpi_maxeax >= 7) {
		struct cpuid_regs *ecp;
		ecp = &cpi->cpi_std[7];
		ecp->cp_eax = 7;
		ecp->cp_ecx = 0;
		(void) __cpuid_insn(ecp);

		/*
		 * If XSAVE has been disabled, just ignore all of the
		 * extended-save-area dependent flags here.
		 */
		if (xsave_force_disable) {
			ecp->cp_ebx &= ~CPUID_INTC_EBX_7_0_BMI1;
			ecp->cp_ebx &= ~CPUID_INTC_EBX_7_0_BMI2;
			ecp->cp_ebx &= ~CPUID_INTC_EBX_7_0_AVX2;
			ecp->cp_ebx &= ~CPUID_INTC_EBX_7_0_MPX;
			ecp->cp_ebx &= ~CPUID_INTC_EBX_7_0_ALL_AVX512;
			ecp->cp_ecx &= ~CPUID_INTC_ECX_7_0_ALL_AVX512;
			ecp->cp_edx &= ~CPUID_INTC_EDX_7_0_ALL_AVX512;
		}

		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_SMEP)
			add_x86_feature(featureset, X86FSET_SMEP);

		/*
		 * We check disable_smap here in addition to in startup_smap()
		 * to ensure CPUs that aren't the boot CPU don't accidentally
		 * include it in the feature set and thus generate a mismatched
		 * x86 feature set across CPUs.
		 */
		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_SMAP &&
		    disable_smap == 0)
			add_x86_feature(featureset, X86FSET_SMAP);

		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_RDSEED)
			add_x86_feature(featureset, X86FSET_RDSEED);

		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_ADX)
			add_x86_feature(featureset, X86FSET_ADX);

		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_FSGSBASE)
			add_x86_feature(featureset, X86FSET_FSGSBASE);

		if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_CLFLUSHOPT)
			add_x86_feature(featureset, X86FSET_CLFLUSHOPT);

		if (cpi->cpi_vendor == X86_VENDOR_Intel) {
			if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_INVPCID)
				add_x86_feature(featureset, X86FSET_INVPCID);

			if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_MPX)
				add_x86_feature(featureset, X86FSET_MPX);

			if (ecp->cp_ebx & CPUID_INTC_EBX_7_0_CLWB)
				add_x86_feature(featureset, X86FSET_CLWB);
		}
	}

	/*
	 * fold in overrides from the "eeprom" mechanism
	 */
	cp->cp_edx |= cpuid_feature_edx_include;
	cp->cp_edx &= ~cpuid_feature_edx_exclude;

	cp->cp_ecx |= cpuid_feature_ecx_include;
	cp->cp_ecx &= ~cpuid_feature_ecx_exclude;

	if (cp->cp_edx & CPUID_INTC_EDX_PSE) {
		add_x86_feature(featureset, X86FSET_LARGEPAGE);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_TSC) {
		add_x86_feature(featureset, X86FSET_TSC);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_MSR) {
		add_x86_feature(featureset, X86FSET_MSR);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_MTRR) {
		add_x86_feature(featureset, X86FSET_MTRR);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_PGE) {
		add_x86_feature(featureset, X86FSET_PGE);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_CMOV) {
		add_x86_feature(featureset, X86FSET_CMOV);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_MMX) {
		add_x86_feature(featureset, X86FSET_MMX);
	}
	if ((cp->cp_edx & CPUID_INTC_EDX_MCE) != 0 &&
	    (cp->cp_edx & CPUID_INTC_EDX_MCA) != 0) {
		add_x86_feature(featureset, X86FSET_MCA);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_PAE) {
		add_x86_feature(featureset, X86FSET_PAE);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_CX8) {
		add_x86_feature(featureset, X86FSET_CX8);
	}
	if (cp->cp_ecx & CPUID_INTC_ECX_CX16) {
		add_x86_feature(featureset, X86FSET_CX16);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_PAT) {
		add_x86_feature(featureset, X86FSET_PAT);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_SEP) {
		add_x86_feature(featureset, X86FSET_SEP);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_FXSR) {
		/*
		 * In our implementation, fxsave/fxrstor
		 * are prerequisites before we'll even
		 * try and do SSE things.
		 */
		if (cp->cp_edx & CPUID_INTC_EDX_SSE) {
			add_x86_feature(featureset, X86FSET_SSE);
		}
		if (cp->cp_edx & CPUID_INTC_EDX_SSE2) {
			add_x86_feature(featureset, X86FSET_SSE2);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_SSE3) {
			add_x86_feature(featureset, X86FSET_SSE3);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_SSSE3) {
			add_x86_feature(featureset, X86FSET_SSSE3);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_SSE4_1) {
			add_x86_feature(featureset, X86FSET_SSE4_1);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_SSE4_2) {
			add_x86_feature(featureset, X86FSET_SSE4_2);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_AES) {
			add_x86_feature(featureset, X86FSET_AES);
		}
		if (cp->cp_ecx & CPUID_INTC_ECX_PCLMULQDQ) {
			add_x86_feature(featureset, X86FSET_PCLMULQDQ);
		}

		if (cpi->cpi_std[7].cp_ebx & CPUID_INTC_EBX_7_0_SHA)
			add_x86_feature(featureset, X86FSET_SHA);

		if (cpi->cpi_std[7].cp_ecx & CPUID_INTC_ECX_7_0_UMIP)
			add_x86_feature(featureset, X86FSET_UMIP);
		if (cpi->cpi_std[7].cp_ecx & CPUID_INTC_ECX_7_0_PKU)
			add_x86_feature(featureset, X86FSET_PKU);
		if (cpi->cpi_std[7].cp_ecx & CPUID_INTC_ECX_7_0_OSPKE)
			add_x86_feature(featureset, X86FSET_OSPKE);

		if (cp->cp_ecx & CPUID_INTC_ECX_XSAVE) {
			add_x86_feature(featureset, X86FSET_XSAVE);

			/* We only test AVX & AVX512 when there is XSAVE */

			if (cp->cp_ecx & CPUID_INTC_ECX_AVX) {
				add_x86_feature(featureset,
				    X86FSET_AVX);

				/*
				 * Intel says we can't check these without also
				 * checking AVX.
				 */
				if (cp->cp_ecx & CPUID_INTC_ECX_F16C)
					add_x86_feature(featureset,
					    X86FSET_F16C);

				if (cp->cp_ecx & CPUID_INTC_ECX_FMA)
					add_x86_feature(featureset,
					    X86FSET_FMA);

				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_BMI1)
					add_x86_feature(featureset,
					    X86FSET_BMI1);

				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_BMI2)
					add_x86_feature(featureset,
					    X86FSET_BMI2);

				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX2)
					add_x86_feature(featureset,
					    X86FSET_AVX2);
			}

			if (cpi->cpi_vendor == X86_VENDOR_Intel &&
			    (cpi->cpi_std[7].cp_ebx &
			    CPUID_INTC_EBX_7_0_AVX512F) != 0) {
				add_x86_feature(featureset, X86FSET_AVX512F);

				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512DQ)
					add_x86_feature(featureset,
					    X86FSET_AVX512DQ);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512IFMA)
					add_x86_feature(featureset,
					    X86FSET_AVX512FMA);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512PF)
					add_x86_feature(featureset,
					    X86FSET_AVX512PF);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512ER)
					add_x86_feature(featureset,
					    X86FSET_AVX512ER);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512CD)
					add_x86_feature(featureset,
					    X86FSET_AVX512CD);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512BW)
					add_x86_feature(featureset,
					    X86FSET_AVX512BW);
				if (cpi->cpi_std[7].cp_ebx &
				    CPUID_INTC_EBX_7_0_AVX512VL)
					add_x86_feature(featureset,
					    X86FSET_AVX512VL);

				if (cpi->cpi_std[7].cp_ecx &
				    CPUID_INTC_ECX_7_0_AVX512VBMI)
					add_x86_feature(featureset,
					    X86FSET_AVX512VBMI);
				if (cpi->cpi_std[7].cp_ecx &
				    CPUID_INTC_ECX_7_0_AVX512VNNI)
					add_x86_feature(featureset,
					    X86FSET_AVX512VNNI);
				if (cpi->cpi_std[7].cp_ecx &
				    CPUID_INTC_ECX_7_0_AVX512VPOPCDQ)
					add_x86_feature(featureset,
					    X86FSET_AVX512VPOPCDQ);

				if (cpi->cpi_std[7].cp_edx &
				    CPUID_INTC_EDX_7_0_AVX5124NNIW)
					add_x86_feature(featureset,
					    X86FSET_AVX512NNIW);
				if (cpi->cpi_std[7].cp_edx &
				    CPUID_INTC_EDX_7_0_AVX5124FMAPS)
					add_x86_feature(featureset,
					    X86FSET_AVX512FMAPS);
			}
		}
	}

	if (cpi->cpi_vendor == X86_VENDOR_Intel) {
		if (cp->cp_ecx & CPUID_INTC_ECX_PCID) {
			add_x86_feature(featureset, X86FSET_PCID);
		}
	}

	if (cp->cp_ecx & CPUID_INTC_ECX_X2APIC) {
		add_x86_feature(featureset, X86FSET_X2APIC);
	}
	if (cp->cp_edx & CPUID_INTC_EDX_DE) {
		add_x86_feature(featureset, X86FSET_DE);
	}
#if !defined(__xpv)
	if (cp->cp_ecx & CPUID_INTC_ECX_MON) {

		/*
		 * We require the CLFLUSH instruction for erratum workaround
		 * to use MONITOR/MWAIT.
		 */
		if (cp->cp_edx & CPUID_INTC_EDX_CLFSH) {
			cpi->cpi_mwait.support |= MWAIT_SUPPORT;
			add_x86_feature(featureset, X86FSET_MWAIT);
		} else {
			extern int idle_cpu_assert_cflush_monitor;

			/*
			 * All processors we are aware of which have
			 * MONITOR/MWAIT also have CLFLUSH.
			 */
			if (idle_cpu_assert_cflush_monitor) {
				ASSERT((cp->cp_ecx & CPUID_INTC_ECX_MON) &&
				    (cp->cp_edx & CPUID_INTC_EDX_CLFSH));
			}
		}
	}
#endif	/* __xpv */

	if (cp->cp_ecx & CPUID_INTC_ECX_VMX) {
		add_x86_feature(featureset, X86FSET_VMX);
	}

	if (cp->cp_ecx & CPUID_INTC_ECX_RDRAND)
		add_x86_feature(featureset, X86FSET_RDRAND);

	/*
	 * Only need it first time, rest of the cpus would follow suit.
	 * we only capture this for the bootcpu.
	 */
	if (cp->cp_edx & CPUID_INTC_EDX_CLFSH) {
		add_x86_feature(featureset, X86FSET_CLFSH);
		x86_clflush_size = (BITX(cp->cp_ebx, 15, 8) * 8);
	}
	if (is_x86_feature(featureset, X86FSET_PAE))
		cpi->cpi_pabits = 36;

	if (cpi->cpi_maxeax >= 0xD && !xsave_force_disable) {
		struct cpuid_regs r, *ecp;

		ecp = &r;
		ecp->cp_eax = 0xD;
		ecp->cp_ecx = 1;
		ecp->cp_edx = ecp->cp_ebx = 0;
		(void) __cpuid_insn(ecp);

		if (ecp->cp_eax & CPUID_INTC_EAX_D_1_XSAVEOPT)
			add_x86_feature(featureset, X86FSET_XSAVEOPT);
		if (ecp->cp_eax & CPUID_INTC_EAX_D_1_XSAVEC)
			add_x86_feature(featureset, X86FSET_XSAVEC);
		if (ecp->cp_eax & CPUID_INTC_EAX_D_1_XSAVES)
			add_x86_feature(featureset, X86FSET_XSAVES);
	}

	/*
	 * Work on the "extended" feature information, doing
	 * some basic initialization for cpuid_pass2()
	 */
	xcpuid = 0;
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		/*
		 * On KVM we know we will have proper support for extended
		 * cpuid.
		 */
		if (IS_NEW_F6(cpi) || cpi->cpi_family >= 0xf ||
		    (get_hwenv() == HW_KVM && cpi->cpi_family == 6 &&
		    (cpi->cpi_model == 6 || cpi->cpi_model == 2)))
			xcpuid++;
		break;
	case X86_VENDOR_AMD:
		if (cpi->cpi_family > 5 ||
		    (cpi->cpi_family == 5 && cpi->cpi_model >= 1))
			xcpuid++;
		break;
	case X86_VENDOR_Cyrix:
		/*
		 * Only these Cyrix CPUs are -known- to support
		 * extended cpuid operations.
		 */
		if (x86_type == X86_TYPE_VIA_CYRIX_III ||
		    x86_type == X86_TYPE_CYRIX_GXm)
			xcpuid++;
		break;
	case X86_VENDOR_Centaur:
	case X86_VENDOR_TM:
	default:
		xcpuid++;
		break;
	}

	if (xcpuid) {
		cp = &cpi->cpi_extd[0];
		cp->cp_eax = CPUID_LEAF_EXT_0;
		cpi->cpi_xmaxeax = __cpuid_insn(cp);
	}

	if (cpi->cpi_xmaxeax & CPUID_LEAF_EXT_0) {

		if (cpi->cpi_xmaxeax > CPI_XMAXEAX_MAX)
			cpi->cpi_xmaxeax = CPI_XMAXEAX_MAX;

		switch (cpi->cpi_vendor) {
		case X86_VENDOR_Intel:
		case X86_VENDOR_AMD:
			if (cpi->cpi_xmaxeax < 0x80000001)
				break;
			cp = &cpi->cpi_extd[1];
			cp->cp_eax = 0x80000001;
			(void) __cpuid_insn(cp);

			if (cpi->cpi_vendor == X86_VENDOR_AMD &&
			    cpi->cpi_family == 5 &&
			    cpi->cpi_model == 6 &&
			    cpi->cpi_step == 6) {
				/*
				 * K6 model 6 uses bit 10 to indicate SYSC
				 * Later models use bit 11. Fix it here.
				 */
				if (cp->cp_edx & 0x400) {
					cp->cp_edx &= ~0x400;
					cp->cp_edx |= CPUID_AMD_EDX_SYSC;
				}
			}

			platform_cpuid_mangle(cpi->cpi_vendor, 0x80000001, cp);

			/*
			 * Compute the additions to the kernel's feature word.
			 */
			if (cp->cp_edx & CPUID_AMD_EDX_NX) {
				add_x86_feature(featureset, X86FSET_NX);
			}

			/*
			 * Regardless whether or not we boot 64-bit,
			 * we should have a way to identify whether
			 * the CPU is capable of running 64-bit.
			 */
			if (cp->cp_edx & CPUID_AMD_EDX_LM) {
				add_x86_feature(featureset, X86FSET_64);
			}

			/* 1 GB large page - enable only for 64 bit kernel */
			if (cp->cp_edx & CPUID_AMD_EDX_1GPG) {
				add_x86_feature(featureset, X86FSET_1GPG);
			}

			if ((cpi->cpi_vendor == X86_VENDOR_AMD) &&
			    (cpi->cpi_std[1].cp_edx & CPUID_INTC_EDX_FXSR) &&
			    (cp->cp_ecx & CPUID_AMD_ECX_SSE4A)) {
				add_x86_feature(featureset, X86FSET_SSE4A);
			}

			/*
			 * It's really tricky to support syscall/sysret in
			 * the i386 kernel; we rely on sysenter/sysexit
			 * instead.  In the amd64 kernel, things are -way-
			 * better.
			 */
			if (cp->cp_edx & CPUID_AMD_EDX_SYSC) {
				add_x86_feature(featureset, X86FSET_ASYSC);
			}

			/*
			 * While we're thinking about system calls, note
			 * that AMD processors don't support sysenter
			 * in long mode at all, so don't try to program them.
			 */
			if (x86_vendor == X86_VENDOR_AMD) {
				remove_x86_feature(featureset, X86FSET_SEP);
			}

			if (cp->cp_edx & CPUID_AMD_EDX_TSCP) {
				add_x86_feature(featureset, X86FSET_TSCP);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_SVM) {
				add_x86_feature(featureset, X86FSET_SVM);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_TOPOEXT) {
				add_x86_feature(featureset, X86FSET_TOPOEXT);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_PCEC) {
				add_x86_feature(featureset, X86FSET_AMD_PCEC);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_XOP) {
				add_x86_feature(featureset, X86FSET_XOP);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_FMA4) {
				add_x86_feature(featureset, X86FSET_FMA4);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_TBM) {
				add_x86_feature(featureset, X86FSET_TBM);
			}

			if (cp->cp_ecx & CPUID_AMD_ECX_MONITORX) {
				add_x86_feature(featureset, X86FSET_MONITORX);
			}
			break;
		default:
			break;
		}

		/*
		 * Get CPUID data about processor cores and hyperthreads.
		 */
		switch (cpi->cpi_vendor) {
		case X86_VENDOR_Intel:
			if (cpi->cpi_maxeax >= 4) {
				cp = &cpi->cpi_std[4];
				cp->cp_eax = 4;
				cp->cp_ecx = 0;
				(void) __cpuid_insn(cp);
				platform_cpuid_mangle(cpi->cpi_vendor, 4, cp);
			}
			/*FALLTHROUGH*/
		case X86_VENDOR_AMD:
			if (cpi->cpi_xmaxeax < CPUID_LEAF_EXT_8)
				break;
			cp = &cpi->cpi_extd[8];
			cp->cp_eax = CPUID_LEAF_EXT_8;
			(void) __cpuid_insn(cp);
			platform_cpuid_mangle(cpi->cpi_vendor, CPUID_LEAF_EXT_8,
			    cp);

			/*
			 * AMD uses ebx for some extended functions.
			 */
			if (cpi->cpi_vendor == X86_VENDOR_AMD) {
				/*
				 * While we're here, check for the AMD "Error
				 * Pointer Zero/Restore" feature. This can be
				 * used to setup the FP save handlers
				 * appropriately.
				 */
				if (cp->cp_ebx & CPUID_AMD_EBX_ERR_PTR_ZERO) {
					cpi->cpi_fp_amd_save = 0;
				} else {
					cpi->cpi_fp_amd_save = 1;
				}

				if (cp->cp_ebx & CPUID_AMD_EBX_CLZERO) {
					add_x86_feature(featureset,
					    X86FSET_CLZERO);
				}
			}

			/*
			 * Virtual and physical address limits from
			 * cpuid override previously guessed values.
			 */
			cpi->cpi_pabits = BITX(cp->cp_eax, 7, 0);
			cpi->cpi_vabits = BITX(cp->cp_eax, 15, 8);
			break;
		default:
			break;
		}

		/*
		 * Get CPUID data about TSC Invariance in Deep C-State.
		 */
		switch (cpi->cpi_vendor) {
		case X86_VENDOR_Intel:
		case X86_VENDOR_AMD:
			if (cpi->cpi_maxeax >= 7) {
				cp = &cpi->cpi_extd[7];
				cp->cp_eax = 0x80000007;
				cp->cp_ecx = 0;
				(void) __cpuid_insn(cp);
			}
			break;
		default:
			break;
		}
	}

	cpuid_pass1_topology(cpu, featureset);
	cpuid_pass1_thermal(cpu, featureset);

	/*
	 * Synthesize chip "revision" and socket type
	 */
	cpi->cpi_chiprev = _cpuid_chiprev(cpi->cpi_vendor, cpi->cpi_family,
	    cpi->cpi_model, cpi->cpi_step);
	cpi->cpi_chiprevstr = _cpuid_chiprevstr(cpi->cpi_vendor,
	    cpi->cpi_family, cpi->cpi_model, cpi->cpi_step);
	cpi->cpi_socket = _cpuid_skt(cpi->cpi_vendor, cpi->cpi_family,
	    cpi->cpi_model, cpi->cpi_step);

	if (cpi->cpi_vendor == X86_VENDOR_AMD) {
		if (cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_8 &&
		    cpi->cpi_extd[8].cp_ebx & CPUID_AMD_EBX_ERR_PTR_ZERO) {
			/* Special handling for AMD FP not necessary. */
			cpi->cpi_fp_amd_save = 0;
		} else {
			cpi->cpi_fp_amd_save = 1;
		}
	}

	/*
	 * Check the processor leaves that are used for security features.
	 */
	cpuid_scan_security(cpu, featureset);

pass1_done:
	cpi->cpi_pass = 1;
}

/*
 * Make copies of the cpuid table entries we depend on, in
 * part for ease of parsing now, in part so that we have only
 * one place to correct any of it, in part for ease of
 * later export to userland, and in part so we can look at
 * this stuff in a crash dump.
 */

/*ARGSUSED*/
void
cpuid_pass2(cpu_t *cpu)
{
	uint_t n, nmax;
	int i;
	struct cpuid_regs *cp;
	uint8_t *dp;
	uint32_t *iptr;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpi->cpi_pass == 1);

	if (cpi->cpi_maxeax < 1)
		goto pass2_done;

	if ((nmax = cpi->cpi_maxeax + 1) > NMAX_CPI_STD)
		nmax = NMAX_CPI_STD;
	/*
	 * (We already handled n == 0 and n == 1 in pass 1)
	 */
	for (n = 2, cp = &cpi->cpi_std[2]; n < nmax; n++, cp++) {
		/*
		 * leaves 6 and 7 were handled in pass 1
		 */
		if (n == 6 || n == 7)
			continue;

		cp->cp_eax = n;

		/*
		 * CPUID function 4 expects %ecx to be initialized
		 * with an index which indicates which cache to return
		 * information about. The OS is expected to call function 4
		 * with %ecx set to 0, 1, 2, ... until it returns with
		 * EAX[4:0] set to 0, which indicates there are no more
		 * caches.
		 *
		 * Here, populate cpi_std[4] with the information returned by
		 * function 4 when %ecx == 0, and do the rest in cpuid_pass3()
		 * when dynamic memory allocation becomes available.
		 *
		 * Note: we need to explicitly initialize %ecx here, since
		 * function 4 may have been previously invoked.
		 */
		if (n == 4)
			cp->cp_ecx = 0;

		(void) __cpuid_insn(cp);
		platform_cpuid_mangle(cpi->cpi_vendor, n, cp);
		switch (n) {
		case 2:
			/*
			 * "the lower 8 bits of the %eax register
			 * contain a value that identifies the number
			 * of times the cpuid [instruction] has to be
			 * executed to obtain a complete image of the
			 * processor's caching systems."
			 *
			 * How *do* they make this stuff up?
			 */
			cpi->cpi_ncache = sizeof (*cp) *
			    BITX(cp->cp_eax, 7, 0);
			if (cpi->cpi_ncache == 0)
				break;
			cpi->cpi_ncache--;	/* skip count byte */

			/*
			 * Well, for now, rather than attempt to implement
			 * this slightly dubious algorithm, we just look
			 * at the first 15 ..
			 */
			if (cpi->cpi_ncache > (sizeof (*cp) - 1))
				cpi->cpi_ncache = sizeof (*cp) - 1;

			dp = cpi->cpi_cacheinfo;
			if (BITX(cp->cp_eax, 31, 31) == 0) {
				uint8_t *p = (void *)&cp->cp_eax;
				for (i = 1; i < 4; i++)
					if (p[i] != 0)
						*dp++ = p[i];
			}
			if (BITX(cp->cp_ebx, 31, 31) == 0) {
				uint8_t *p = (void *)&cp->cp_ebx;
				for (i = 0; i < 4; i++)
					if (p[i] != 0)
						*dp++ = p[i];
			}
			if (BITX(cp->cp_ecx, 31, 31) == 0) {
				uint8_t *p = (void *)&cp->cp_ecx;
				for (i = 0; i < 4; i++)
					if (p[i] != 0)
						*dp++ = p[i];
			}
			if (BITX(cp->cp_edx, 31, 31) == 0) {
				uint8_t *p = (void *)&cp->cp_edx;
				for (i = 0; i < 4; i++)
					if (p[i] != 0)
						*dp++ = p[i];
			}
			break;

		case 3:	/* Processor serial number, if PSN supported */
			break;

		case 4:	/* Deterministic cache parameters */
			break;

		case 5:	/* Monitor/Mwait parameters */
		{
			size_t mwait_size;

			/*
			 * check cpi_mwait.support which was set in cpuid_pass1
			 */
			if (!(cpi->cpi_mwait.support & MWAIT_SUPPORT))
				break;

			/*
			 * Protect ourself from insane mwait line size.
			 * Workaround for incomplete hardware emulator(s).
			 */
			mwait_size = (size_t)MWAIT_SIZE_MAX(cpi);
			if (mwait_size < sizeof (uint32_t) ||
			    !ISP2(mwait_size)) {
#if DEBUG
				cmn_err(CE_NOTE, "Cannot handle cpu %d mwait "
				    "size %ld", cpu->cpu_id, (long)mwait_size);
#endif
				break;
			}

			cpi->cpi_mwait.mon_min = (size_t)MWAIT_SIZE_MIN(cpi);
			cpi->cpi_mwait.mon_max = mwait_size;
			if (MWAIT_EXTENSION(cpi)) {
				cpi->cpi_mwait.support |= MWAIT_EXTENSIONS;
				if (MWAIT_INT_ENABLE(cpi))
					cpi->cpi_mwait.support |=
					    MWAIT_ECX_INT_ENABLE;
			}
			break;
		}
		default:
			break;
		}
	}

	/*
	 * XSAVE enumeration
	 */
	if (cpi->cpi_maxeax >= 0xD) {
		struct cpuid_regs regs;
		boolean_t cpuid_d_valid = B_TRUE;

		cp = &regs;
		cp->cp_eax = 0xD;
		cp->cp_edx = cp->cp_ebx = cp->cp_ecx = 0;

		(void) __cpuid_insn(cp);

		/*
		 * Sanity checks for debug
		 */
		if ((cp->cp_eax & XFEATURE_LEGACY_FP) == 0 ||
		    (cp->cp_eax & XFEATURE_SSE) == 0) {
			cpuid_d_valid = B_FALSE;
		}

		cpi->cpi_xsave.xsav_hw_features_low = cp->cp_eax;
		cpi->cpi_xsave.xsav_hw_features_high = cp->cp_edx;
		cpi->cpi_xsave.xsav_max_size = cp->cp_ecx;

		/*
		 * If the hw supports AVX, get the size and offset in the save
		 * area for the ymm state.
		 */
		if (cpi->cpi_xsave.xsav_hw_features_low & XFEATURE_AVX) {
			cp->cp_eax = 0xD;
			cp->cp_ecx = 2;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			if (cp->cp_ebx != CPUID_LEAFD_2_YMM_OFFSET ||
			    cp->cp_eax != CPUID_LEAFD_2_YMM_SIZE) {
				cpuid_d_valid = B_FALSE;
			}

			cpi->cpi_xsave.ymm_size = cp->cp_eax;
			cpi->cpi_xsave.ymm_offset = cp->cp_ebx;
		}

		/*
		 * If the hw supports MPX, get the size and offset in the
		 * save area for BNDREGS and BNDCSR.
		 */
		if (cpi->cpi_xsave.xsav_hw_features_low & XFEATURE_MPX) {
			cp->cp_eax = 0xD;
			cp->cp_ecx = 3;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			cpi->cpi_xsave.bndregs_size = cp->cp_eax;
			cpi->cpi_xsave.bndregs_offset = cp->cp_ebx;

			cp->cp_eax = 0xD;
			cp->cp_ecx = 4;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			cpi->cpi_xsave.bndcsr_size = cp->cp_eax;
			cpi->cpi_xsave.bndcsr_offset = cp->cp_ebx;
		}

		/*
		 * If the hw supports AVX512, get the size and offset in the
		 * save area for the opmask registers and zmm state.
		 */
		if (cpi->cpi_xsave.xsav_hw_features_low & XFEATURE_AVX512) {
			cp->cp_eax = 0xD;
			cp->cp_ecx = 5;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			cpi->cpi_xsave.opmask_size = cp->cp_eax;
			cpi->cpi_xsave.opmask_offset = cp->cp_ebx;

			cp->cp_eax = 0xD;
			cp->cp_ecx = 6;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			cpi->cpi_xsave.zmmlo_size = cp->cp_eax;
			cpi->cpi_xsave.zmmlo_offset = cp->cp_ebx;

			cp->cp_eax = 0xD;
			cp->cp_ecx = 7;
			cp->cp_edx = cp->cp_ebx = 0;

			(void) __cpuid_insn(cp);

			cpi->cpi_xsave.zmmhi_size = cp->cp_eax;
			cpi->cpi_xsave.zmmhi_offset = cp->cp_ebx;
		}

		if (is_x86_feature(x86_featureset, X86FSET_XSAVE)) {
			xsave_state_size = 0;
		} else if (cpuid_d_valid) {
			xsave_state_size = cpi->cpi_xsave.xsav_max_size;
		} else {
			/* Broken CPUID 0xD, probably in HVM */
			cmn_err(CE_WARN, "cpu%d: CPUID.0xD returns invalid "
			    "value: hw_low = %d, hw_high = %d, xsave_size = %d"
			    ", ymm_size = %d, ymm_offset = %d\n",
			    cpu->cpu_id, cpi->cpi_xsave.xsav_hw_features_low,
			    cpi->cpi_xsave.xsav_hw_features_high,
			    (int)cpi->cpi_xsave.xsav_max_size,
			    (int)cpi->cpi_xsave.ymm_size,
			    (int)cpi->cpi_xsave.ymm_offset);

			if (xsave_state_size != 0) {
				/*
				 * This must be a non-boot CPU. We cannot
				 * continue, because boot cpu has already
				 * enabled XSAVE.
				 */
				ASSERT(cpu->cpu_id != 0);
				cmn_err(CE_PANIC, "cpu%d: we have already "
				    "enabled XSAVE on boot cpu, cannot "
				    "continue.", cpu->cpu_id);
			} else {
				/*
				 * If we reached here on the boot CPU, it's also
				 * almost certain that we'll reach here on the
				 * non-boot CPUs. When we're here on a boot CPU
				 * we should disable the feature, on a non-boot
				 * CPU we need to confirm that we have.
				 */
				if (cpu->cpu_id == 0) {
					remove_x86_feature(x86_featureset,
					    X86FSET_XSAVE);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX);
					remove_x86_feature(x86_featureset,
					    X86FSET_F16C);
					remove_x86_feature(x86_featureset,
					    X86FSET_BMI1);
					remove_x86_feature(x86_featureset,
					    X86FSET_BMI2);
					remove_x86_feature(x86_featureset,
					    X86FSET_FMA);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX2);
					remove_x86_feature(x86_featureset,
					    X86FSET_MPX);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512F);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512DQ);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512PF);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512ER);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512CD);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512BW);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512VL);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512FMA);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512VBMI);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512VNNI);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512VPOPCDQ);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512NNIW);
					remove_x86_feature(x86_featureset,
					    X86FSET_AVX512FMAPS);

					CPI_FEATURES_ECX(cpi) &=
					    ~CPUID_INTC_ECX_XSAVE;
					CPI_FEATURES_ECX(cpi) &=
					    ~CPUID_INTC_ECX_AVX;
					CPI_FEATURES_ECX(cpi) &=
					    ~CPUID_INTC_ECX_F16C;
					CPI_FEATURES_ECX(cpi) &=
					    ~CPUID_INTC_ECX_FMA;
					CPI_FEATURES_7_0_EBX(cpi) &=
					    ~CPUID_INTC_EBX_7_0_BMI1;
					CPI_FEATURES_7_0_EBX(cpi) &=
					    ~CPUID_INTC_EBX_7_0_BMI2;
					CPI_FEATURES_7_0_EBX(cpi) &=
					    ~CPUID_INTC_EBX_7_0_AVX2;
					CPI_FEATURES_7_0_EBX(cpi) &=
					    ~CPUID_INTC_EBX_7_0_MPX;
					CPI_FEATURES_7_0_EBX(cpi) &=
					    ~CPUID_INTC_EBX_7_0_ALL_AVX512;

					CPI_FEATURES_7_0_ECX(cpi) &=
					    ~CPUID_INTC_ECX_7_0_ALL_AVX512;

					CPI_FEATURES_7_0_EDX(cpi) &=
					    ~CPUID_INTC_EDX_7_0_ALL_AVX512;

					xsave_force_disable = B_TRUE;
				} else {
					VERIFY(is_x86_feature(x86_featureset,
					    X86FSET_XSAVE) == B_FALSE);
				}
			}
		}
	}


	if ((cpi->cpi_xmaxeax & CPUID_LEAF_EXT_0) == 0)
		goto pass2_done;

	if ((nmax = cpi->cpi_xmaxeax - CPUID_LEAF_EXT_0 + 1) > NMAX_CPI_EXTD)
		nmax = NMAX_CPI_EXTD;
	/*
	 * Copy the extended properties, fixing them as we go.
	 * (We already handled n == 0 and n == 1 in pass 1)
	 */
	iptr = (void *)cpi->cpi_brandstr;
	for (n = 2, cp = &cpi->cpi_extd[2]; n < nmax; cp++, n++) {
		cp->cp_eax = CPUID_LEAF_EXT_0 + n;
		(void) __cpuid_insn(cp);
		platform_cpuid_mangle(cpi->cpi_vendor, CPUID_LEAF_EXT_0 + n,
		    cp);
		switch (n) {
		case 2:
		case 3:
		case 4:
			/*
			 * Extract the brand string
			 */
			*iptr++ = cp->cp_eax;
			*iptr++ = cp->cp_ebx;
			*iptr++ = cp->cp_ecx;
			*iptr++ = cp->cp_edx;
			break;
		case 5:
			switch (cpi->cpi_vendor) {
			case X86_VENDOR_AMD:
				/*
				 * The Athlon and Duron were the first
				 * parts to report the sizes of the
				 * TLB for large pages. Before then,
				 * we don't trust the data.
				 */
				if (cpi->cpi_family < 6 ||
				    (cpi->cpi_family == 6 &&
				    cpi->cpi_model < 1))
					cp->cp_eax = 0;
				break;
			default:
				break;
			}
			break;
		case 6:
			switch (cpi->cpi_vendor) {
			case X86_VENDOR_AMD:
				/*
				 * The Athlon and Duron were the first
				 * AMD parts with L2 TLB's.
				 * Before then, don't trust the data.
				 */
				if (cpi->cpi_family < 6 ||
				    cpi->cpi_family == 6 &&
				    cpi->cpi_model < 1)
					cp->cp_eax = cp->cp_ebx = 0;
				/*
				 * AMD Duron rev A0 reports L2
				 * cache size incorrectly as 1K
				 * when it is really 64K
				 */
				if (cpi->cpi_family == 6 &&
				    cpi->cpi_model == 3 &&
				    cpi->cpi_step == 0) {
					cp->cp_ecx &= 0xffff;
					cp->cp_ecx |= 0x400000;
				}
				break;
			case X86_VENDOR_Cyrix:	/* VIA C3 */
				/*
				 * VIA C3 processors are a bit messed
				 * up w.r.t. encoding cache sizes in %ecx
				 */
				if (cpi->cpi_family != 6)
					break;
				/*
				 * model 7 and 8 were incorrectly encoded
				 *
				 * xxx is model 8 really broken?
				 */
				if (cpi->cpi_model == 7 ||
				    cpi->cpi_model == 8)
					cp->cp_ecx =
					    BITX(cp->cp_ecx, 31, 24) << 16 |
					    BITX(cp->cp_ecx, 23, 16) << 12 |
					    BITX(cp->cp_ecx, 15, 8) << 8 |
					    BITX(cp->cp_ecx, 7, 0);
				/*
				 * model 9 stepping 1 has wrong associativity
				 */
				if (cpi->cpi_model == 9 && cpi->cpi_step == 1)
					cp->cp_ecx |= 8 << 12;
				break;
			case X86_VENDOR_Intel:
				/*
				 * Extended L2 Cache features function.
				 * First appeared on Prescott.
				 */
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

pass2_done:
	cpi->cpi_pass = 2;
}

static const char *
intel_cpubrand(const struct cpuid_info *cpi)
{
	int i;

	if (!is_x86_feature(x86_featureset, X86FSET_CPUID) ||
	    cpi->cpi_maxeax < 1 || cpi->cpi_family < 5)
		return ("i486");

	switch (cpi->cpi_family) {
	case 5:
		return ("Intel Pentium(r)");
	case 6:
		switch (cpi->cpi_model) {
			uint_t celeron, xeon;
			const struct cpuid_regs *cp;
		case 0:
		case 1:
		case 2:
			return ("Intel Pentium(r) Pro");
		case 3:
		case 4:
			return ("Intel Pentium(r) II");
		case 6:
			return ("Intel Celeron(r)");
		case 5:
		case 7:
			celeron = xeon = 0;
			cp = &cpi->cpi_std[2];	/* cache info */

			for (i = 1; i < 4; i++) {
				uint_t tmp;

				tmp = (cp->cp_eax >> (8 * i)) & 0xff;
				if (tmp == 0x40)
					celeron++;
				if (tmp >= 0x44 && tmp <= 0x45)
					xeon++;
			}

			for (i = 0; i < 2; i++) {
				uint_t tmp;

				tmp = (cp->cp_ebx >> (8 * i)) & 0xff;
				if (tmp == 0x40)
					celeron++;
				else if (tmp >= 0x44 && tmp <= 0x45)
					xeon++;
			}

			for (i = 0; i < 4; i++) {
				uint_t tmp;

				tmp = (cp->cp_ecx >> (8 * i)) & 0xff;
				if (tmp == 0x40)
					celeron++;
				else if (tmp >= 0x44 && tmp <= 0x45)
					xeon++;
			}

			for (i = 0; i < 4; i++) {
				uint_t tmp;

				tmp = (cp->cp_edx >> (8 * i)) & 0xff;
				if (tmp == 0x40)
					celeron++;
				else if (tmp >= 0x44 && tmp <= 0x45)
					xeon++;
			}

			if (celeron)
				return ("Intel Celeron(r)");
			if (xeon)
				return (cpi->cpi_model == 5 ?
				    "Intel Pentium(r) II Xeon(tm)" :
				    "Intel Pentium(r) III Xeon(tm)");
			return (cpi->cpi_model == 5 ?
			    "Intel Pentium(r) II or Pentium(r) II Xeon(tm)" :
			    "Intel Pentium(r) III or Pentium(r) III Xeon(tm)");
		default:
			break;
		}
	default:
		break;
	}

	/* BrandID is present if the field is nonzero */
	if (cpi->cpi_brandid != 0) {
		static const struct {
			uint_t bt_bid;
			const char *bt_str;
		} brand_tbl[] = {
			{ 0x1,	"Intel(r) Celeron(r)" },
			{ 0x2,	"Intel(r) Pentium(r) III" },
			{ 0x3,	"Intel(r) Pentium(r) III Xeon(tm)" },
			{ 0x4,	"Intel(r) Pentium(r) III" },
			{ 0x6,	"Mobile Intel(r) Pentium(r) III" },
			{ 0x7,	"Mobile Intel(r) Celeron(r)" },
			{ 0x8,	"Intel(r) Pentium(r) 4" },
			{ 0x9,	"Intel(r) Pentium(r) 4" },
			{ 0xa,	"Intel(r) Celeron(r)" },
			{ 0xb,	"Intel(r) Xeon(tm)" },
			{ 0xc,	"Intel(r) Xeon(tm) MP" },
			{ 0xe,	"Mobile Intel(r) Pentium(r) 4" },
			{ 0xf,	"Mobile Intel(r) Celeron(r)" },
			{ 0x11, "Mobile Genuine Intel(r)" },
			{ 0x12, "Intel(r) Celeron(r) M" },
			{ 0x13, "Mobile Intel(r) Celeron(r)" },
			{ 0x14, "Intel(r) Celeron(r)" },
			{ 0x15, "Mobile Genuine Intel(r)" },
			{ 0x16,	"Intel(r) Pentium(r) M" },
			{ 0x17, "Mobile Intel(r) Celeron(r)" }
		};
		uint_t btblmax = sizeof (brand_tbl) / sizeof (brand_tbl[0]);
		uint_t sgn;

		sgn = (cpi->cpi_family << 8) |
		    (cpi->cpi_model << 4) | cpi->cpi_step;

		for (i = 0; i < btblmax; i++)
			if (brand_tbl[i].bt_bid == cpi->cpi_brandid)
				break;
		if (i < btblmax) {
			if (sgn == 0x6b1 && cpi->cpi_brandid == 3)
				return ("Intel(r) Celeron(r)");
			if (sgn < 0xf13 && cpi->cpi_brandid == 0xb)
				return ("Intel(r) Xeon(tm) MP");
			if (sgn < 0xf13 && cpi->cpi_brandid == 0xe)
				return ("Intel(r) Xeon(tm)");
			return (brand_tbl[i].bt_str);
		}
	}

	return (NULL);
}

static const char *
amd_cpubrand(const struct cpuid_info *cpi)
{
	if (!is_x86_feature(x86_featureset, X86FSET_CPUID) ||
	    cpi->cpi_maxeax < 1 || cpi->cpi_family < 5)
		return ("i486 compatible");

	switch (cpi->cpi_family) {
	case 5:
		switch (cpi->cpi_model) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return ("AMD-K5(r)");
		case 6:
		case 7:
			return ("AMD-K6(r)");
		case 8:
			return ("AMD-K6(r)-2");
		case 9:
			return ("AMD-K6(r)-III");
		default:
			return ("AMD (family 5)");
		}
	case 6:
		switch (cpi->cpi_model) {
		case 1:
			return ("AMD-K7(tm)");
		case 0:
		case 2:
		case 4:
			return ("AMD Athlon(tm)");
		case 3:
		case 7:
			return ("AMD Duron(tm)");
		case 6:
		case 8:
		case 10:
			/*
			 * Use the L2 cache size to distinguish
			 */
			return ((cpi->cpi_extd[6].cp_ecx >> 16) >= 256 ?
			    "AMD Athlon(tm)" : "AMD Duron(tm)");
		default:
			return ("AMD (family 6)");
		}
	default:
		break;
	}

	if (cpi->cpi_family == 0xf && cpi->cpi_model == 5 &&
	    cpi->cpi_brandid != 0) {
		switch (BITX(cpi->cpi_brandid, 7, 5)) {
		case 3:
			return ("AMD Opteron(tm) UP 1xx");
		case 4:
			return ("AMD Opteron(tm) DP 2xx");
		case 5:
			return ("AMD Opteron(tm) MP 8xx");
		default:
			return ("AMD Opteron(tm)");
		}
	}

	return (NULL);
}

static const char *
cyrix_cpubrand(struct cpuid_info *cpi, uint_t type)
{
	if (!is_x86_feature(x86_featureset, X86FSET_CPUID) ||
	    cpi->cpi_maxeax < 1 || cpi->cpi_family < 5 ||
	    type == X86_TYPE_CYRIX_486)
		return ("i486 compatible");

	switch (type) {
	case X86_TYPE_CYRIX_6x86:
		return ("Cyrix 6x86");
	case X86_TYPE_CYRIX_6x86L:
		return ("Cyrix 6x86L");
	case X86_TYPE_CYRIX_6x86MX:
		return ("Cyrix 6x86MX");
	case X86_TYPE_CYRIX_GXm:
		return ("Cyrix GXm");
	case X86_TYPE_CYRIX_MediaGX:
		return ("Cyrix MediaGX");
	case X86_TYPE_CYRIX_MII:
		return ("Cyrix M2");
	case X86_TYPE_VIA_CYRIX_III:
		return ("VIA Cyrix M3");
	default:
		/*
		 * Have another wild guess ..
		 */
		if (cpi->cpi_family == 4 && cpi->cpi_model == 9)
			return ("Cyrix 5x86");
		else if (cpi->cpi_family == 5) {
			switch (cpi->cpi_model) {
			case 2:
				return ("Cyrix 6x86");	/* Cyrix M1 */
			case 4:
				return ("Cyrix MediaGX");
			default:
				break;
			}
		} else if (cpi->cpi_family == 6) {
			switch (cpi->cpi_model) {
			case 0:
				return ("Cyrix 6x86MX"); /* Cyrix M2? */
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				return ("VIA C3");
			default:
				break;
			}
		}
		break;
	}
	return (NULL);
}

/*
 * This only gets called in the case that the CPU extended
 * feature brand string (0x80000002, 0x80000003, 0x80000004)
 * aren't available, or contain null bytes for some reason.
 */
static void
fabricate_brandstr(struct cpuid_info *cpi)
{
	const char *brand = NULL;

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		brand = intel_cpubrand(cpi);
		break;
	case X86_VENDOR_AMD:
		brand = amd_cpubrand(cpi);
		break;
	case X86_VENDOR_Cyrix:
		brand = cyrix_cpubrand(cpi, x86_type);
		break;
	case X86_VENDOR_NexGen:
		if (cpi->cpi_family == 5 && cpi->cpi_model == 0)
			brand = "NexGen Nx586";
		break;
	case X86_VENDOR_Centaur:
		if (cpi->cpi_family == 5)
			switch (cpi->cpi_model) {
			case 4:
				brand = "Centaur C6";
				break;
			case 8:
				brand = "Centaur C2";
				break;
			case 9:
				brand = "Centaur C3";
				break;
			default:
				break;
			}
		break;
	case X86_VENDOR_Rise:
		if (cpi->cpi_family == 5 &&
		    (cpi->cpi_model == 0 || cpi->cpi_model == 2))
			brand = "Rise mP6";
		break;
	case X86_VENDOR_SiS:
		if (cpi->cpi_family == 5 && cpi->cpi_model == 0)
			brand = "SiS 55x";
		break;
	case X86_VENDOR_TM:
		if (cpi->cpi_family == 5 && cpi->cpi_model == 4)
			brand = "Transmeta Crusoe TM3x00 or TM5x00";
		break;
	case X86_VENDOR_NSC:
	case X86_VENDOR_UMC:
	default:
		break;
	}
	if (brand) {
		(void) strcpy((char *)cpi->cpi_brandstr, brand);
		return;
	}

	/*
	 * If all else fails ...
	 */
	(void) snprintf(cpi->cpi_brandstr, sizeof (cpi->cpi_brandstr),
	    "%s %d.%d.%d", cpi->cpi_vendorstr, cpi->cpi_family,
	    cpi->cpi_model, cpi->cpi_step);
}

/*
 * This routine is called just after kernel memory allocation
 * becomes available on cpu0, and as part of mp_startup() on
 * the other cpus.
 *
 * Fixup the brand string, and collect any information from cpuid
 * that requires dynamically allocated storage to represent.
 */
/*ARGSUSED*/
void
cpuid_pass3(cpu_t *cpu)
{
	int	i, max, shft, level, size;
	struct cpuid_regs regs;
	struct cpuid_regs *cp;
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpi->cpi_pass == 2);

	/*
	 * Deterministic cache parameters
	 *
	 * Intel uses leaf 0x4 for this, while AMD uses leaf 0x8000001d. The
	 * values that are present are currently defined to be the same. This
	 * means we can use the same logic to parse it as long as we use the
	 * appropriate leaf to get the data. If you're updating this, make sure
	 * you're careful about which vendor supports which aspect.
	 *
	 * Take this opportunity to detect the number of threads sharing the
	 * last level cache, and construct a corresponding cache id. The
	 * respective cpuid_info members are initialized to the default case of
	 * "no last level cache sharing".
	 */
	cpi->cpi_ncpu_shr_last_cache = 1;
	cpi->cpi_last_lvl_cacheid = cpu->cpu_id;

	if ((cpi->cpi_maxeax >= 4 && cpi->cpi_vendor == X86_VENDOR_Intel) ||
	    (cpi->cpi_vendor == X86_VENDOR_AMD &&
	    cpi->cpi_xmaxeax >= CPUID_LEAF_EXT_1d &&
	    is_x86_feature(x86_featureset, X86FSET_TOPOEXT))) {
		uint32_t leaf;

		if (cpi->cpi_vendor == X86_VENDOR_Intel) {
			leaf = 4;
		} else {
			leaf = CPUID_LEAF_EXT_1d;
		}

		/*
		 * Find the # of elements (size) returned by the leaf and along
		 * the way detect last level cache sharing details.
		 */
		bzero(&regs, sizeof (regs));
		cp = &regs;
		for (i = 0, max = 0; i < CPI_FN4_ECX_MAX; i++) {
			cp->cp_eax = leaf;
			cp->cp_ecx = i;

			(void) __cpuid_insn(cp);

			if (CPI_CACHE_TYPE(cp) == 0)
				break;
			level = CPI_CACHE_LVL(cp);
			if (level > max) {
				max = level;
				cpi->cpi_ncpu_shr_last_cache =
				    CPI_NTHR_SHR_CACHE(cp) + 1;
			}
		}
		cpi->cpi_cache_leaf_size = size = i;

		/*
		 * Allocate the cpi_cache_leaves array. The first element
		 * references the regs for the corresponding leaf with %ecx set
		 * to 0. This was gathered in cpuid_pass2().
		 */
		if (size > 0) {
			cpi->cpi_cache_leaves =
			    kmem_alloc(size * sizeof (cp), KM_SLEEP);
			if (cpi->cpi_vendor == X86_VENDOR_Intel) {
				cpi->cpi_cache_leaves[0] = &cpi->cpi_std[4];
			} else {
				cpi->cpi_cache_leaves[0] = &cpi->cpi_extd[0x1d];
			}

			/*
			 * Allocate storage to hold the additional regs
			 * for the leaf, %ecx == 1 .. cpi_cache_leaf_size.
			 *
			 * The regs for the leaf, %ecx == 0 has already
			 * been allocated as indicated above.
			 */
			for (i = 1; i < size; i++) {
				cp = cpi->cpi_cache_leaves[i] =
				    kmem_zalloc(sizeof (regs), KM_SLEEP);
				cp->cp_eax = leaf;
				cp->cp_ecx = i;

				(void) __cpuid_insn(cp);
			}
		}
		/*
		 * Determine the number of bits needed to represent
		 * the number of CPUs sharing the last level cache.
		 *
		 * Shift off that number of bits from the APIC id to
		 * derive the cache id.
		 */
		shft = 0;
		for (i = 1; i < cpi->cpi_ncpu_shr_last_cache; i <<= 1)
			shft++;
		cpi->cpi_last_lvl_cacheid = cpi->cpi_apicid >> shft;
	}

	/*
	 * Now fixup the brand string
	 */
	if ((cpi->cpi_xmaxeax & CPUID_LEAF_EXT_0) == 0) {
		fabricate_brandstr(cpi);
	} else {

		/*
		 * If we successfully extracted a brand string from the cpuid
		 * instruction, clean it up by removing leading spaces and
		 * similar junk.
		 */
		if (cpi->cpi_brandstr[0]) {
			size_t maxlen = sizeof (cpi->cpi_brandstr);
			char *src, *dst;

			dst = src = (char *)cpi->cpi_brandstr;
			src[maxlen - 1] = '\0';
			/*
			 * strip leading spaces
			 */
			while (*src == ' ')
				src++;
			/*
			 * Remove any 'Genuine' or "Authentic" prefixes
			 */
			if (strncmp(src, "Genuine ", 8) == 0)
				src += 8;
			if (strncmp(src, "Authentic ", 10) == 0)
				src += 10;

			/*
			 * Now do an in-place copy.
			 * Map (R) to (r) and (TM) to (tm).
			 * The era of teletypes is long gone, and there's
			 * -really- no need to shout.
			 */
			while (*src != '\0') {
				if (src[0] == '(') {
					if (strncmp(src + 1, "R)", 2) == 0) {
						(void) strncpy(dst, "(r)", 3);
						src += 3;
						dst += 3;
						continue;
					}
					if (strncmp(src + 1, "TM)", 3) == 0) {
						(void) strncpy(dst, "(tm)", 4);
						src += 4;
						dst += 4;
						continue;
					}
				}
				*dst++ = *src++;
			}
			*dst = '\0';

			/*
			 * Finally, remove any trailing spaces
			 */
			while (--dst > cpi->cpi_brandstr)
				if (*dst == ' ')
					*dst = '\0';
				else
					break;
		} else
			fabricate_brandstr(cpi);
	}
	cpi->cpi_pass = 3;
}

/*
 * This routine is called out of bind_hwcap() much later in the life
 * of the kernel (post_startup()).  The job of this routine is to resolve
 * the hardware feature support and kernel support for those features into
 * what we're actually going to tell applications via the aux vector.
 */
void
cpuid_pass4(cpu_t *cpu, uint_t *hwcap_out)
{
	struct cpuid_info *cpi;
	uint_t hwcap_flags = 0, hwcap_flags_2 = 0;

	if (cpu == NULL)
		cpu = CPU;
	cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpi->cpi_pass == 3);

	if (cpi->cpi_maxeax >= 1) {
		uint32_t *edx = &cpi->cpi_support[STD_EDX_FEATURES];
		uint32_t *ecx = &cpi->cpi_support[STD_ECX_FEATURES];
		uint32_t *ebx = &cpi->cpi_support[STD_EBX_FEATURES];

		*edx = CPI_FEATURES_EDX(cpi);
		*ecx = CPI_FEATURES_ECX(cpi);
		*ebx = CPI_FEATURES_7_0_EBX(cpi);

		/*
		 * [these require explicit kernel support]
		 */
		if (!is_x86_feature(x86_featureset, X86FSET_SEP))
			*edx &= ~CPUID_INTC_EDX_SEP;

		if (!is_x86_feature(x86_featureset, X86FSET_SSE))
			*edx &= ~(CPUID_INTC_EDX_FXSR|CPUID_INTC_EDX_SSE);
		if (!is_x86_feature(x86_featureset, X86FSET_SSE2))
			*edx &= ~CPUID_INTC_EDX_SSE2;

		if (!is_x86_feature(x86_featureset, X86FSET_HTT))
			*edx &= ~CPUID_INTC_EDX_HTT;

		if (!is_x86_feature(x86_featureset, X86FSET_SSE3))
			*ecx &= ~CPUID_INTC_ECX_SSE3;

		if (!is_x86_feature(x86_featureset, X86FSET_SSSE3))
			*ecx &= ~CPUID_INTC_ECX_SSSE3;
		if (!is_x86_feature(x86_featureset, X86FSET_SSE4_1))
			*ecx &= ~CPUID_INTC_ECX_SSE4_1;
		if (!is_x86_feature(x86_featureset, X86FSET_SSE4_2))
			*ecx &= ~CPUID_INTC_ECX_SSE4_2;
		if (!is_x86_feature(x86_featureset, X86FSET_AES))
			*ecx &= ~CPUID_INTC_ECX_AES;
		if (!is_x86_feature(x86_featureset, X86FSET_PCLMULQDQ))
			*ecx &= ~CPUID_INTC_ECX_PCLMULQDQ;
		if (!is_x86_feature(x86_featureset, X86FSET_XSAVE))
			*ecx &= ~(CPUID_INTC_ECX_XSAVE |
			    CPUID_INTC_ECX_OSXSAVE);
		if (!is_x86_feature(x86_featureset, X86FSET_AVX))
			*ecx &= ~CPUID_INTC_ECX_AVX;
		if (!is_x86_feature(x86_featureset, X86FSET_F16C))
			*ecx &= ~CPUID_INTC_ECX_F16C;
		if (!is_x86_feature(x86_featureset, X86FSET_FMA))
			*ecx &= ~CPUID_INTC_ECX_FMA;
		if (!is_x86_feature(x86_featureset, X86FSET_BMI1))
			*ebx &= ~CPUID_INTC_EBX_7_0_BMI1;
		if (!is_x86_feature(x86_featureset, X86FSET_BMI2))
			*ebx &= ~CPUID_INTC_EBX_7_0_BMI2;
		if (!is_x86_feature(x86_featureset, X86FSET_AVX2))
			*ebx &= ~CPUID_INTC_EBX_7_0_AVX2;
		if (!is_x86_feature(x86_featureset, X86FSET_RDSEED))
			*ebx &= ~CPUID_INTC_EBX_7_0_RDSEED;
		if (!is_x86_feature(x86_featureset, X86FSET_ADX))
			*ebx &= ~CPUID_INTC_EBX_7_0_ADX;

		/*
		 * [no explicit support required beyond x87 fp context]
		 */
		if (!fpu_exists)
			*edx &= ~(CPUID_INTC_EDX_FPU | CPUID_INTC_EDX_MMX);

		/*
		 * Now map the supported feature vector to things that we
		 * think userland will care about.
		 */
		if (*edx & CPUID_INTC_EDX_SEP)
			hwcap_flags |= AV_386_SEP;
		if (*edx & CPUID_INTC_EDX_SSE)
			hwcap_flags |= AV_386_FXSR | AV_386_SSE;
		if (*edx & CPUID_INTC_EDX_SSE2)
			hwcap_flags |= AV_386_SSE2;
		if (*ecx & CPUID_INTC_ECX_SSE3)
			hwcap_flags |= AV_386_SSE3;
		if (*ecx & CPUID_INTC_ECX_SSSE3)
			hwcap_flags |= AV_386_SSSE3;
		if (*ecx & CPUID_INTC_ECX_SSE4_1)
			hwcap_flags |= AV_386_SSE4_1;
		if (*ecx & CPUID_INTC_ECX_SSE4_2)
			hwcap_flags |= AV_386_SSE4_2;
		if (*ecx & CPUID_INTC_ECX_MOVBE)
			hwcap_flags |= AV_386_MOVBE;
		if (*ecx & CPUID_INTC_ECX_AES)
			hwcap_flags |= AV_386_AES;
		if (*ecx & CPUID_INTC_ECX_PCLMULQDQ)
			hwcap_flags |= AV_386_PCLMULQDQ;
		if ((*ecx & CPUID_INTC_ECX_XSAVE) &&
		    (*ecx & CPUID_INTC_ECX_OSXSAVE)) {
			hwcap_flags |= AV_386_XSAVE;

			if (*ecx & CPUID_INTC_ECX_AVX) {
				uint32_t *ecx_7 = &CPI_FEATURES_7_0_ECX(cpi);
				uint32_t *edx_7 = &CPI_FEATURES_7_0_EDX(cpi);

				hwcap_flags |= AV_386_AVX;
				if (*ecx & CPUID_INTC_ECX_F16C)
					hwcap_flags_2 |= AV_386_2_F16C;
				if (*ecx & CPUID_INTC_ECX_FMA)
					hwcap_flags_2 |= AV_386_2_FMA;

				if (*ebx & CPUID_INTC_EBX_7_0_BMI1)
					hwcap_flags_2 |= AV_386_2_BMI1;
				if (*ebx & CPUID_INTC_EBX_7_0_BMI2)
					hwcap_flags_2 |= AV_386_2_BMI2;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX2)
					hwcap_flags_2 |= AV_386_2_AVX2;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512F)
					hwcap_flags_2 |= AV_386_2_AVX512F;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512DQ)
					hwcap_flags_2 |= AV_386_2_AVX512DQ;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512IFMA)
					hwcap_flags_2 |= AV_386_2_AVX512IFMA;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512PF)
					hwcap_flags_2 |= AV_386_2_AVX512PF;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512ER)
					hwcap_flags_2 |= AV_386_2_AVX512ER;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512CD)
					hwcap_flags_2 |= AV_386_2_AVX512CD;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512BW)
					hwcap_flags_2 |= AV_386_2_AVX512BW;
				if (*ebx & CPUID_INTC_EBX_7_0_AVX512VL)
					hwcap_flags_2 |= AV_386_2_AVX512VL;

				if (*ecx_7 & CPUID_INTC_ECX_7_0_AVX512VBMI)
					hwcap_flags_2 |= AV_386_2_AVX512VBMI;
				if (*ecx_7 & CPUID_INTC_ECX_7_0_AVX512VNNI)
					hwcap_flags_2 |= AV_386_2_AVX512_VNNI;
				if (*ecx_7 & CPUID_INTC_ECX_7_0_AVX512VPOPCDQ)
					hwcap_flags_2 |= AV_386_2_AVX512VPOPCDQ;

				if (*edx_7 & CPUID_INTC_EDX_7_0_AVX5124NNIW)
					hwcap_flags_2 |= AV_386_2_AVX512_4NNIW;
				if (*edx_7 & CPUID_INTC_EDX_7_0_AVX5124FMAPS)
					hwcap_flags_2 |= AV_386_2_AVX512_4FMAPS;
			}
		}
		if (*ecx & CPUID_INTC_ECX_VMX)
			hwcap_flags |= AV_386_VMX;
		if (*ecx & CPUID_INTC_ECX_POPCNT)
			hwcap_flags |= AV_386_POPCNT;
		if (*edx & CPUID_INTC_EDX_FPU)
			hwcap_flags |= AV_386_FPU;
		if (*edx & CPUID_INTC_EDX_MMX)
			hwcap_flags |= AV_386_MMX;

		if (*edx & CPUID_INTC_EDX_TSC)
			hwcap_flags |= AV_386_TSC;
		if (*edx & CPUID_INTC_EDX_CX8)
			hwcap_flags |= AV_386_CX8;
		if (*edx & CPUID_INTC_EDX_CMOV)
			hwcap_flags |= AV_386_CMOV;
		if (*ecx & CPUID_INTC_ECX_CX16)
			hwcap_flags |= AV_386_CX16;

		if (*ecx & CPUID_INTC_ECX_RDRAND)
			hwcap_flags_2 |= AV_386_2_RDRAND;
		if (*ebx & CPUID_INTC_EBX_7_0_ADX)
			hwcap_flags_2 |= AV_386_2_ADX;
		if (*ebx & CPUID_INTC_EBX_7_0_RDSEED)
			hwcap_flags_2 |= AV_386_2_RDSEED;
		if (*ebx & CPUID_INTC_EBX_7_0_SHA)
			hwcap_flags_2 |= AV_386_2_SHA;
		if (*ebx & CPUID_INTC_EBX_7_0_FSGSBASE)
			hwcap_flags_2 |= AV_386_2_FSGSBASE;
		if (*ebx & CPUID_INTC_EBX_7_0_CLWB)
			hwcap_flags_2 |= AV_386_2_CLWB;
		if (*ebx & CPUID_INTC_EBX_7_0_CLFLUSHOPT)
			hwcap_flags_2 |= AV_386_2_CLFLUSHOPT;

	}
	/*
	 * Check a few miscilaneous features.
	 */
	if (is_x86_feature(x86_featureset, X86FSET_CLZERO))
		hwcap_flags_2 |= AV_386_2_CLZERO;

	if (cpi->cpi_xmaxeax < 0x80000001)
		goto pass4_done;

	switch (cpi->cpi_vendor) {
		struct cpuid_regs cp;
		uint32_t *edx, *ecx;

	case X86_VENDOR_Intel:
		/*
		 * Seems like Intel duplicated what we necessary
		 * here to make the initial crop of 64-bit OS's work.
		 * Hopefully, those are the only "extended" bits
		 * they'll add.
		 */
		/*FALLTHROUGH*/

	case X86_VENDOR_AMD:
		edx = &cpi->cpi_support[AMD_EDX_FEATURES];
		ecx = &cpi->cpi_support[AMD_ECX_FEATURES];

		*edx = CPI_FEATURES_XTD_EDX(cpi);
		*ecx = CPI_FEATURES_XTD_ECX(cpi);

		/*
		 * [these features require explicit kernel support]
		 */
		switch (cpi->cpi_vendor) {
		case X86_VENDOR_Intel:
			if (!is_x86_feature(x86_featureset, X86FSET_TSCP))
				*edx &= ~CPUID_AMD_EDX_TSCP;
			break;

		case X86_VENDOR_AMD:
			if (!is_x86_feature(x86_featureset, X86FSET_TSCP))
				*edx &= ~CPUID_AMD_EDX_TSCP;
			if (!is_x86_feature(x86_featureset, X86FSET_SSE4A))
				*ecx &= ~CPUID_AMD_ECX_SSE4A;
			break;

		default:
			break;
		}

		/*
		 * [no explicit support required beyond
		 * x87 fp context and exception handlers]
		 */
		if (!fpu_exists)
			*edx &= ~(CPUID_AMD_EDX_MMXamd |
			    CPUID_AMD_EDX_3DNow | CPUID_AMD_EDX_3DNowx);

		if (!is_x86_feature(x86_featureset, X86FSET_NX))
			*edx &= ~CPUID_AMD_EDX_NX;
#if !defined(__amd64)
		*edx &= ~CPUID_AMD_EDX_LM;
#endif
		/*
		 * Now map the supported feature vector to
		 * things that we think userland will care about.
		 */
#if defined(__amd64)
		if (*edx & CPUID_AMD_EDX_SYSC)
			hwcap_flags |= AV_386_AMD_SYSC;
#endif
		if (*edx & CPUID_AMD_EDX_MMXamd)
			hwcap_flags |= AV_386_AMD_MMX;
		if (*edx & CPUID_AMD_EDX_3DNow)
			hwcap_flags |= AV_386_AMD_3DNow;
		if (*edx & CPUID_AMD_EDX_3DNowx)
			hwcap_flags |= AV_386_AMD_3DNowx;
		if (*ecx & CPUID_AMD_ECX_SVM)
			hwcap_flags |= AV_386_AMD_SVM;

		switch (cpi->cpi_vendor) {
		case X86_VENDOR_AMD:
			if (*edx & CPUID_AMD_EDX_TSCP)
				hwcap_flags |= AV_386_TSCP;
			if (*ecx & CPUID_AMD_ECX_AHF64)
				hwcap_flags |= AV_386_AHF;
			if (*ecx & CPUID_AMD_ECX_SSE4A)
				hwcap_flags |= AV_386_AMD_SSE4A;
			if (*ecx & CPUID_AMD_ECX_LZCNT)
				hwcap_flags |= AV_386_AMD_LZCNT;
			if (*ecx & CPUID_AMD_ECX_MONITORX)
				hwcap_flags_2 |= AV_386_2_MONITORX;
			break;

		case X86_VENDOR_Intel:
			if (*edx & CPUID_AMD_EDX_TSCP)
				hwcap_flags |= AV_386_TSCP;
			if (*ecx & CPUID_AMD_ECX_LZCNT)
				hwcap_flags |= AV_386_AMD_LZCNT;
			/*
			 * Aarrgh.
			 * Intel uses a different bit in the same word.
			 */
			if (*ecx & CPUID_INTC_ECX_AHF64)
				hwcap_flags |= AV_386_AHF;
			break;

		default:
			break;
		}
		break;

	case X86_VENDOR_TM:
		cp.cp_eax = 0x80860001;
		(void) __cpuid_insn(&cp);
		cpi->cpi_support[TM_EDX_FEATURES] = cp.cp_edx;
		break;

	default:
		break;
	}

pass4_done:
	cpi->cpi_pass = 4;
	if (hwcap_out != NULL) {
		hwcap_out[0] = hwcap_flags;
		hwcap_out[1] = hwcap_flags_2;
	}
}


/*
 * Simulate the cpuid instruction using the data we previously
 * captured about this CPU.  We try our best to return the truth
 * about the hardware, independently of kernel support.
 */
uint32_t
cpuid_insn(cpu_t *cpu, struct cpuid_regs *cp)
{
	struct cpuid_info *cpi;
	struct cpuid_regs *xcp;

	if (cpu == NULL)
		cpu = CPU;
	cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpuid_checkpass(cpu, 3));

	/*
	 * CPUID data is cached in two separate places: cpi_std for standard
	 * CPUID leaves , and cpi_extd for extended CPUID leaves.
	 */
	if (cp->cp_eax <= cpi->cpi_maxeax && cp->cp_eax < NMAX_CPI_STD) {
		xcp = &cpi->cpi_std[cp->cp_eax];
	} else if (cp->cp_eax >= CPUID_LEAF_EXT_0 &&
	    cp->cp_eax <= cpi->cpi_xmaxeax &&
	    cp->cp_eax < CPUID_LEAF_EXT_0 + NMAX_CPI_EXTD) {
		xcp = &cpi->cpi_extd[cp->cp_eax - CPUID_LEAF_EXT_0];
	} else {
		/*
		 * The caller is asking for data from an input parameter which
		 * the kernel has not cached.  In this case we go fetch from
		 * the hardware and return the data directly to the user.
		 */
		return (__cpuid_insn(cp));
	}

	cp->cp_eax = xcp->cp_eax;
	cp->cp_ebx = xcp->cp_ebx;
	cp->cp_ecx = xcp->cp_ecx;
	cp->cp_edx = xcp->cp_edx;
	return (cp->cp_eax);
}

int
cpuid_checkpass(cpu_t *cpu, int pass)
{
	return (cpu != NULL && cpu->cpu_m.mcpu_cpi != NULL &&
	    cpu->cpu_m.mcpu_cpi->cpi_pass >= pass);
}

int
cpuid_getbrandstr(cpu_t *cpu, char *s, size_t n)
{
	ASSERT(cpuid_checkpass(cpu, 3));

	return (snprintf(s, n, "%s", cpu->cpu_m.mcpu_cpi->cpi_brandstr));
}

int
cpuid_is_cmt(cpu_t *cpu)
{
	if (cpu == NULL)
		cpu = CPU;

	ASSERT(cpuid_checkpass(cpu, 1));

	return (cpu->cpu_m.mcpu_cpi->cpi_chipid >= 0);
}

/*
 * AMD and Intel both implement the 64-bit variant of the syscall
 * instruction (syscallq), so if there's -any- support for syscall,
 * cpuid currently says "yes, we support this".
 *
 * However, Intel decided to -not- implement the 32-bit variant of the
 * syscall instruction, so we provide a predicate to allow our caller
 * to test that subtlety here.
 *
 * XXPV	Currently, 32-bit syscall instructions don't work via the hypervisor,
 *	even in the case where the hardware would in fact support it.
 */
/*ARGSUSED*/
int
cpuid_syscall32_insn(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass((cpu == NULL ? CPU : cpu), 1));

#if !defined(__xpv)
	if (cpu == NULL)
		cpu = CPU;

	/*CSTYLED*/
	{
		struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

		if (cpi->cpi_vendor == X86_VENDOR_AMD &&
		    cpi->cpi_xmaxeax >= 0x80000001 &&
		    (CPI_FEATURES_XTD_EDX(cpi) & CPUID_AMD_EDX_SYSC))
			return (1);
	}
#endif
	return (0);
}

int
cpuid_getidstr(cpu_t *cpu, char *s, size_t n)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;

	static const char fmt[] =
	    "x86 (%s %X family %d model %d step %d clock %d MHz)";
	static const char fmt_ht[] =
	    "x86 (chipid 0x%x %s %X family %d model %d step %d clock %d MHz)";

	ASSERT(cpuid_checkpass(cpu, 1));

	if (cpuid_is_cmt(cpu))
		return (snprintf(s, n, fmt_ht, cpi->cpi_chipid,
		    cpi->cpi_vendorstr, cpi->cpi_std[1].cp_eax,
		    cpi->cpi_family, cpi->cpi_model,
		    cpi->cpi_step, cpu->cpu_type_info.pi_clock));
	return (snprintf(s, n, fmt,
	    cpi->cpi_vendorstr, cpi->cpi_std[1].cp_eax,
	    cpi->cpi_family, cpi->cpi_model,
	    cpi->cpi_step, cpu->cpu_type_info.pi_clock));
}

const char *
cpuid_getvendorstr(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return ((const char *)cpu->cpu_m.mcpu_cpi->cpi_vendorstr);
}

uint_t
cpuid_getvendor(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_vendor);
}

uint_t
cpuid_getfamily(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_family);
}

uint_t
cpuid_getmodel(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_model);
}

uint_t
cpuid_get_ncpu_per_chip(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_ncpu_per_chip);
}

uint_t
cpuid_get_ncore_per_chip(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_ncore_per_chip);
}

uint_t
cpuid_get_ncpu_sharing_last_cache(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 2));
	return (cpu->cpu_m.mcpu_cpi->cpi_ncpu_shr_last_cache);
}

id_t
cpuid_get_last_lvl_cacheid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 2));
	return (cpu->cpu_m.mcpu_cpi->cpi_last_lvl_cacheid);
}

uint_t
cpuid_getstep(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_step);
}

uint_t
cpuid_getsig(struct cpu *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_std[1].cp_eax);
}

uint32_t
cpuid_getchiprev(struct cpu *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_chiprev);
}

const char *
cpuid_getchiprevstr(struct cpu *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_chiprevstr);
}

uint32_t
cpuid_getsockettype(struct cpu *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_socket);
}

const char *
cpuid_getsocketstr(cpu_t *cpu)
{
	static const char *socketstr = NULL;
	struct cpuid_info *cpi;

	ASSERT(cpuid_checkpass(cpu, 1));
	cpi = cpu->cpu_m.mcpu_cpi;

	/* Assume that socket types are the same across the system */
	if (socketstr == NULL)
		socketstr = _cpuid_sktstr(cpi->cpi_vendor, cpi->cpi_family,
		    cpi->cpi_model, cpi->cpi_step);


	return (socketstr);
}

int
cpuid_get_chipid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));

	if (cpuid_is_cmt(cpu))
		return (cpu->cpu_m.mcpu_cpi->cpi_chipid);
	return (cpu->cpu_id);
}

id_t
cpuid_get_coreid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_coreid);
}

int
cpuid_get_pkgcoreid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_pkgcoreid);
}

int
cpuid_get_clogid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_clogid);
}

int
cpuid_get_cacheid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_last_lvl_cacheid);
}

uint_t
cpuid_get_procnodeid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_procnodeid);
}

uint_t
cpuid_get_procnodes_per_pkg(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_procnodes_per_pkg);
}

uint_t
cpuid_get_compunitid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_compunitid);
}

uint_t
cpuid_get_cores_per_compunit(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	return (cpu->cpu_m.mcpu_cpi->cpi_cores_per_compunit);
}

/*ARGSUSED*/
int
cpuid_have_cr8access(cpu_t *cpu)
{
#if defined(__amd64)
	return (1);
#else
	struct cpuid_info *cpi;

	ASSERT(cpu != NULL);
	cpi = cpu->cpu_m.mcpu_cpi;
	if (cpi->cpi_vendor == X86_VENDOR_AMD && cpi->cpi_maxeax >= 1 &&
	    (CPI_FEATURES_XTD_ECX(cpi) & CPUID_AMD_ECX_CR8D) != 0)
		return (1);
	return (0);
#endif
}

uint32_t
cpuid_get_apicid(cpu_t *cpu)
{
	ASSERT(cpuid_checkpass(cpu, 1));
	if (cpu->cpu_m.mcpu_cpi->cpi_maxeax < 1) {
		return (UINT32_MAX);
	} else {
		return (cpu->cpu_m.mcpu_cpi->cpi_apicid);
	}
}

void
cpuid_get_addrsize(cpu_t *cpu, uint_t *pabits, uint_t *vabits)
{
	struct cpuid_info *cpi;

	if (cpu == NULL)
		cpu = CPU;
	cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpuid_checkpass(cpu, 1));

	if (pabits)
		*pabits = cpi->cpi_pabits;
	if (vabits)
		*vabits = cpi->cpi_vabits;
}

size_t
cpuid_get_xsave_size()
{
	return (MAX(cpuid_info0.cpi_xsave.xsav_max_size,
	    sizeof (struct xsave_state)));
}

/*
 * Return true if the CPUs on this system require 'pointer clearing' for the
 * floating point error pointer exception handling. In the past, this has been
 * true for all AMD K7 & K8 CPUs, although newer AMD CPUs have been changed to
 * behave the same as Intel. This is checked via the CPUID_AMD_EBX_ERR_PTR_ZERO
 * feature bit and is reflected in the cpi_fp_amd_save member.
 */
boolean_t
cpuid_need_fp_excp_handling()
{
	return (cpuid_info0.cpi_vendor == X86_VENDOR_AMD &&
	    cpuid_info0.cpi_fp_amd_save != 0);
}

/*
 * Returns the number of data TLB entries for a corresponding
 * pagesize.  If it can't be computed, or isn't known, the
 * routine returns zero.  If you ask about an architecturally
 * impossible pagesize, the routine will panic (so that the
 * hat implementor knows that things are inconsistent.)
 */
uint_t
cpuid_get_dtlb_nent(cpu_t *cpu, size_t pagesize)
{
	struct cpuid_info *cpi;
	uint_t dtlb_nent = 0;

	if (cpu == NULL)
		cpu = CPU;
	cpi = cpu->cpu_m.mcpu_cpi;

	ASSERT(cpuid_checkpass(cpu, 1));

	/*
	 * Check the L2 TLB info
	 */
	if (cpi->cpi_xmaxeax >= 0x80000006) {
		struct cpuid_regs *cp = &cpi->cpi_extd[6];

		switch (pagesize) {

		case 4 * 1024:
			/*
			 * All zero in the top 16 bits of the register
			 * indicates a unified TLB. Size is in low 16 bits.
			 */
			if ((cp->cp_ebx & 0xffff0000) == 0)
				dtlb_nent = cp->cp_ebx & 0x0000ffff;
			else
				dtlb_nent = BITX(cp->cp_ebx, 27, 16);
			break;

		case 2 * 1024 * 1024:
			if ((cp->cp_eax & 0xffff0000) == 0)
				dtlb_nent = cp->cp_eax & 0x0000ffff;
			else
				dtlb_nent = BITX(cp->cp_eax, 27, 16);
			break;

		default:
			panic("unknown L2 pagesize");
			/*NOTREACHED*/
		}
	}

	if (dtlb_nent != 0)
		return (dtlb_nent);

	/*
	 * No L2 TLB support for this size, try L1.
	 */
	if (cpi->cpi_xmaxeax >= 0x80000005) {
		struct cpuid_regs *cp = &cpi->cpi_extd[5];

		switch (pagesize) {
		case 4 * 1024:
			dtlb_nent = BITX(cp->cp_ebx, 23, 16);
			break;
		case 2 * 1024 * 1024:
			dtlb_nent = BITX(cp->cp_eax, 23, 16);
			break;
		default:
			panic("unknown L1 d-TLB pagesize");
			/*NOTREACHED*/
		}
	}

	return (dtlb_nent);
}

/*
 * Return 0 if the erratum is not present or not applicable, positive
 * if it is, and negative if the status of the erratum is unknown.
 *
 * See "Revision Guide for AMD Athlon(tm) 64 and AMD Opteron(tm)
 * Processors" #25759, Rev 3.57, August 2005
 */
int
cpuid_opteron_erratum(cpu_t *cpu, uint_t erratum)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	uint_t eax;

	/*
	 * Bail out if this CPU isn't an AMD CPU, or if it's
	 * a legacy (32-bit) AMD CPU.
	 */
	if (cpi->cpi_vendor != X86_VENDOR_AMD ||
	    cpi->cpi_family == 4 || cpi->cpi_family == 5 ||
	    cpi->cpi_family == 6) {
		return (0);
	}

	eax = cpi->cpi_std[1].cp_eax;

#define	SH_B0(eax)	(eax == 0xf40 || eax == 0xf50)
#define	SH_B3(eax)	(eax == 0xf51)
#define	B(eax)		(SH_B0(eax) || SH_B3(eax))

#define	SH_C0(eax)	(eax == 0xf48 || eax == 0xf58)

#define	SH_CG(eax)	(eax == 0xf4a || eax == 0xf5a || eax == 0xf7a)
#define	DH_CG(eax)	(eax == 0xfc0 || eax == 0xfe0 || eax == 0xff0)
#define	CH_CG(eax)	(eax == 0xf82 || eax == 0xfb2)
#define	CG(eax)		(SH_CG(eax) || DH_CG(eax) || CH_CG(eax))

#define	SH_D0(eax)	(eax == 0x10f40 || eax == 0x10f50 || eax == 0x10f70)
#define	DH_D0(eax)	(eax == 0x10fc0 || eax == 0x10ff0)
#define	CH_D0(eax)	(eax == 0x10f80 || eax == 0x10fb0)
#define	D0(eax)		(SH_D0(eax) || DH_D0(eax) || CH_D0(eax))

#define	SH_E0(eax)	(eax == 0x20f50 || eax == 0x20f40 || eax == 0x20f70)
#define	JH_E1(eax)	(eax == 0x20f10)	/* JH8_E0 had 0x20f30 */
#define	DH_E3(eax)	(eax == 0x20fc0 || eax == 0x20ff0)
#define	SH_E4(eax)	(eax == 0x20f51 || eax == 0x20f71)
#define	BH_E4(eax)	(eax == 0x20fb1)
#define	SH_E5(eax)	(eax == 0x20f42)
#define	DH_E6(eax)	(eax == 0x20ff2 || eax == 0x20fc2)
#define	JH_E6(eax)	(eax == 0x20f12 || eax == 0x20f32)
#define	EX(eax)		(SH_E0(eax) || JH_E1(eax) || DH_E3(eax) || \
			    SH_E4(eax) || BH_E4(eax) || SH_E5(eax) || \
			    DH_E6(eax) || JH_E6(eax))

#define	DR_AX(eax)	(eax == 0x100f00 || eax == 0x100f01 || eax == 0x100f02)
#define	DR_B0(eax)	(eax == 0x100f20)
#define	DR_B1(eax)	(eax == 0x100f21)
#define	DR_BA(eax)	(eax == 0x100f2a)
#define	DR_B2(eax)	(eax == 0x100f22)
#define	DR_B3(eax)	(eax == 0x100f23)
#define	RB_C0(eax)	(eax == 0x100f40)

	switch (erratum) {
	case 1:
		return (cpi->cpi_family < 0x10);
	case 51:	/* what does the asterisk mean? */
		return (B(eax) || SH_C0(eax) || CG(eax));
	case 52:
		return (B(eax));
	case 57:
		return (cpi->cpi_family <= 0x11);
	case 58:
		return (B(eax));
	case 60:
		return (cpi->cpi_family <= 0x11);
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 68:
	case 69:
	case 70:
	case 71:
		return (B(eax));
	case 72:
		return (SH_B0(eax));
	case 74:
		return (B(eax));
	case 75:
		return (cpi->cpi_family < 0x10);
	case 76:
		return (B(eax));
	case 77:
		return (cpi->cpi_family <= 0x11);
	case 78:
		return (B(eax) || SH_C0(eax));
	case 79:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax) || EX(eax));
	case 80:
	case 81:
	case 82:
		return (B(eax));
	case 83:
		return (B(eax) || SH_C0(eax) || CG(eax));
	case 85:
		return (cpi->cpi_family < 0x10);
	case 86:
		return (SH_C0(eax) || CG(eax));
	case 88:
#if !defined(__amd64)
		return (0);
#else
		return (B(eax) || SH_C0(eax));
#endif
	case 89:
		return (cpi->cpi_family < 0x10);
	case 90:
		return (B(eax) || SH_C0(eax) || CG(eax));
	case 91:
	case 92:
		return (B(eax) || SH_C0(eax));
	case 93:
		return (SH_C0(eax));
	case 94:
		return (B(eax) || SH_C0(eax) || CG(eax));
	case 95:
#if !defined(__amd64)
		return (0);
#else
		return (B(eax) || SH_C0(eax));
#endif
	case 96:
		return (B(eax) || SH_C0(eax) || CG(eax));
	case 97:
	case 98:
		return (SH_C0(eax) || CG(eax));
	case 99:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax));
	case 100:
		return (B(eax) || SH_C0(eax));
	case 101:
	case 103:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax));
	case 104:
		return (SH_C0(eax) || CG(eax) || D0(eax));
	case 105:
	case 106:
	case 107:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax));
	case 108:
		return (DH_CG(eax));
	case 109:
		return (SH_C0(eax) || CG(eax) || D0(eax));
	case 110:
		return (D0(eax) || EX(eax));
	case 111:
		return (CG(eax));
	case 112:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax) || EX(eax));
	case 113:
		return (eax == 0x20fc0);
	case 114:
		return (SH_E0(eax) || JH_E1(eax) || DH_E3(eax));
	case 115:
		return (SH_E0(eax) || JH_E1(eax));
	case 116:
		return (SH_E0(eax) || JH_E1(eax) || DH_E3(eax));
	case 117:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax));
	case 118:
		return (SH_E0(eax) || JH_E1(eax) || SH_E4(eax) || BH_E4(eax) ||
		    JH_E6(eax));
	case 121:
		return (B(eax) || SH_C0(eax) || CG(eax) || D0(eax) || EX(eax));
	case 122:
		return (cpi->cpi_family < 0x10 || cpi->cpi_family == 0x11);
	case 123:
		return (JH_E1(eax) || BH_E4(eax) || JH_E6(eax));
	case 131:
		return (cpi->cpi_family < 0x10);
	case 6336786:

		/*
		 * Test for AdvPowerMgmtInfo.TscPStateInvariant
		 * if this is a K8 family or newer processor. We're testing for
		 * this 'erratum' to determine whether or not we have a constant
		 * TSC.
		 *
		 * Our current fix for this is to disable the C1-Clock ramping.
		 * However, this doesn't work on newer processor families nor
		 * does it work when virtualized as those devices don't exist.
		 */
		if (cpi->cpi_family >= 0x12 || get_hwenv() != HW_NATIVE) {
			return (0);
		}

		if (CPI_FAMILY(cpi) == 0xf) {
			struct cpuid_regs regs;
			regs.cp_eax = 0x80000007;
			(void) __cpuid_insn(&regs);
			return (!(regs.cp_edx & 0x100));
		}
		return (0);
	case 6323525:
		/*
		 * This erratum (K8 #147) is not present on family 10 and newer.
		 */
		if (cpi->cpi_family >= 0x10) {
			return (0);
		}
		return (((((eax >> 12) & 0xff00) + (eax & 0xf00)) |
		    (((eax >> 4) & 0xf) | ((eax >> 12) & 0xf0))) < 0xf40);

	case 6671130:
		/*
		 * check for processors (pre-Shanghai) that do not provide
		 * optimal management of 1gb ptes in its tlb.
		 */
		return (cpi->cpi_family == 0x10 && cpi->cpi_model < 4);

	case 298:
		return (DR_AX(eax) || DR_B0(eax) || DR_B1(eax) || DR_BA(eax) ||
		    DR_B2(eax) || RB_C0(eax));

	case 721:
#if defined(__amd64)
		return (cpi->cpi_family == 0x10 || cpi->cpi_family == 0x12);
#else
		return (0);
#endif

	default:
		return (-1);

	}
}

/*
 * Determine if specified erratum is present via OSVW (OS Visible Workaround).
 * Return 1 if erratum is present, 0 if not present and -1 if indeterminate.
 */
int
osvw_opteron_erratum(cpu_t *cpu, uint_t erratum)
{
	struct cpuid_info	*cpi;
	uint_t			osvwid;
	static int		osvwfeature = -1;
	uint64_t		osvwlength;


	cpi = cpu->cpu_m.mcpu_cpi;

	/* confirm OSVW supported */
	if (osvwfeature == -1) {
		osvwfeature = cpi->cpi_extd[1].cp_ecx & CPUID_AMD_ECX_OSVW;
	} else {
		/* assert that osvw feature setting is consistent on all cpus */
		ASSERT(osvwfeature ==
		    (cpi->cpi_extd[1].cp_ecx & CPUID_AMD_ECX_OSVW));
	}
	if (!osvwfeature)
		return (-1);

	osvwlength = rdmsr(MSR_AMD_OSVW_ID_LEN) & OSVW_ID_LEN_MASK;

	switch (erratum) {
	case 298:	/* osvwid is 0 */
		osvwid = 0;
		if (osvwlength <= (uint64_t)osvwid) {
			/* osvwid 0 is unknown */
			return (-1);
		}

		/*
		 * Check the OSVW STATUS MSR to determine the state
		 * of the erratum where:
		 *   0 - fixed by HW
		 *   1 - BIOS has applied the workaround when BIOS
		 *   workaround is available. (Or for other errata,
		 *   OS workaround is required.)
		 * For a value of 1, caller will confirm that the
		 * erratum 298 workaround has indeed been applied by BIOS.
		 *
		 * A 1 may be set in cpus that have a HW fix
		 * in a mixed cpu system. Regarding erratum 298:
		 *   In a multiprocessor platform, the workaround above
		 *   should be applied to all processors regardless of
		 *   silicon revision when an affected processor is
		 *   present.
		 */

		return (rdmsr(MSR_AMD_OSVW_STATUS +
		    (osvwid / OSVW_ID_CNT_PER_MSR)) &
		    (1ULL << (osvwid % OSVW_ID_CNT_PER_MSR)));

	default:
		return (-1);
	}
}

static const char assoc_str[] = "associativity";
static const char line_str[] = "line-size";
static const char size_str[] = "size";

static void
add_cache_prop(dev_info_t *devi, const char *label, const char *type,
    uint32_t val)
{
	char buf[128];

	/*
	 * ndi_prop_update_int() is used because it is desirable for
	 * DDI_PROP_HW_DEF and DDI_PROP_DONTSLEEP to be set.
	 */
	if (snprintf(buf, sizeof (buf), "%s-%s", label, type) < sizeof (buf))
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, devi, buf, val);
}

/*
 * Intel-style cache/tlb description
 *
 * Standard cpuid level 2 gives a randomly ordered
 * selection of tags that index into a table that describes
 * cache and tlb properties.
 */

static const char l1_icache_str[] = "l1-icache";
static const char l1_dcache_str[] = "l1-dcache";
static const char l2_cache_str[] = "l2-cache";
static const char l3_cache_str[] = "l3-cache";
static const char itlb4k_str[] = "itlb-4K";
static const char dtlb4k_str[] = "dtlb-4K";
static const char itlb2M_str[] = "itlb-2M";
static const char itlb4M_str[] = "itlb-4M";
static const char dtlb4M_str[] = "dtlb-4M";
static const char dtlb24_str[] = "dtlb0-2M-4M";
static const char itlb424_str[] = "itlb-4K-2M-4M";
static const char itlb24_str[] = "itlb-2M-4M";
static const char dtlb44_str[] = "dtlb-4K-4M";
static const char sl1_dcache_str[] = "sectored-l1-dcache";
static const char sl2_cache_str[] = "sectored-l2-cache";
static const char itrace_str[] = "itrace-cache";
static const char sl3_cache_str[] = "sectored-l3-cache";
static const char sh_l2_tlb4k_str[] = "shared-l2-tlb-4k";

static const struct cachetab {
	uint8_t		ct_code;
	uint8_t		ct_assoc;
	uint16_t	ct_line_size;
	size_t		ct_size;
	const char	*ct_label;
} intel_ctab[] = {
	/*
	 * maintain descending order!
	 *
	 * Codes ignored - Reason
	 * ----------------------
	 * 40H - intel_cpuid_4_cache_info() disambiguates l2/l3 cache
	 * f0H/f1H - Currently we do not interpret prefetch size by design
	 */
	{ 0xe4, 16, 64, 8*1024*1024, l3_cache_str},
	{ 0xe3, 16, 64, 4*1024*1024, l3_cache_str},
	{ 0xe2, 16, 64, 2*1024*1024, l3_cache_str},
	{ 0xde, 12, 64, 6*1024*1024, l3_cache_str},
	{ 0xdd, 12, 64, 3*1024*1024, l3_cache_str},
	{ 0xdc, 12, 64, ((1*1024*1024)+(512*1024)), l3_cache_str},
	{ 0xd8, 8, 64, 4*1024*1024, l3_cache_str},
	{ 0xd7, 8, 64, 2*1024*1024, l3_cache_str},
	{ 0xd6, 8, 64, 1*1024*1024, l3_cache_str},
	{ 0xd2, 4, 64, 2*1024*1024, l3_cache_str},
	{ 0xd1, 4, 64, 1*1024*1024, l3_cache_str},
	{ 0xd0, 4, 64, 512*1024, l3_cache_str},
	{ 0xca, 4, 0, 512, sh_l2_tlb4k_str},
	{ 0xc0, 4, 0, 8, dtlb44_str },
	{ 0xba, 4, 0, 64, dtlb4k_str },
	{ 0xb4, 4, 0, 256, dtlb4k_str },
	{ 0xb3, 4, 0, 128, dtlb4k_str },
	{ 0xb2, 4, 0, 64, itlb4k_str },
	{ 0xb0, 4, 0, 128, itlb4k_str },
	{ 0x87, 8, 64, 1024*1024, l2_cache_str},
	{ 0x86, 4, 64, 512*1024, l2_cache_str},
	{ 0x85, 8, 32, 2*1024*1024, l2_cache_str},
	{ 0x84, 8, 32, 1024*1024, l2_cache_str},
	{ 0x83, 8, 32, 512*1024, l2_cache_str},
	{ 0x82, 8, 32, 256*1024, l2_cache_str},
	{ 0x80, 8, 64, 512*1024, l2_cache_str},
	{ 0x7f, 2, 64, 512*1024, l2_cache_str},
	{ 0x7d, 8, 64, 2*1024*1024, sl2_cache_str},
	{ 0x7c, 8, 64, 1024*1024, sl2_cache_str},
	{ 0x7b, 8, 64, 512*1024, sl2_cache_str},
	{ 0x7a, 8, 64, 256*1024, sl2_cache_str},
	{ 0x79, 8, 64, 128*1024, sl2_cache_str},
	{ 0x78, 8, 64, 1024*1024, l2_cache_str},
	{ 0x73, 8, 0, 64*1024, itrace_str},
	{ 0x72, 8, 0, 32*1024, itrace_str},
	{ 0x71, 8, 0, 16*1024, itrace_str},
	{ 0x70, 8, 0, 12*1024, itrace_str},
	{ 0x68, 4, 64, 32*1024, sl1_dcache_str},
	{ 0x67, 4, 64, 16*1024, sl1_dcache_str},
	{ 0x66, 4, 64, 8*1024, sl1_dcache_str},
	{ 0x60, 8, 64, 16*1024, sl1_dcache_str},
	{ 0x5d, 0, 0, 256, dtlb44_str},
	{ 0x5c, 0, 0, 128, dtlb44_str},
	{ 0x5b, 0, 0, 64, dtlb44_str},
	{ 0x5a, 4, 0, 32, dtlb24_str},
	{ 0x59, 0, 0, 16, dtlb4k_str},
	{ 0x57, 4, 0, 16, dtlb4k_str},
	{ 0x56, 4, 0, 16, dtlb4M_str},
	{ 0x55, 0, 0, 7, itlb24_str},
	{ 0x52, 0, 0, 256, itlb424_str},
	{ 0x51, 0, 0, 128, itlb424_str},
	{ 0x50, 0, 0, 64, itlb424_str},
	{ 0x4f, 0, 0, 32, itlb4k_str},
	{ 0x4e, 24, 64, 6*1024*1024, l2_cache_str},
	{ 0x4d, 16, 64, 16*1024*1024, l3_cache_str},
	{ 0x4c, 12, 64, 12*1024*1024, l3_cache_str},
	{ 0x4b, 16, 64, 8*1024*1024, l3_cache_str},
	{ 0x4a, 12, 64, 6*1024*1024, l3_cache_str},
	{ 0x49, 16, 64, 4*1024*1024, l3_cache_str},
	{ 0x48, 12, 64, 3*1024*1024, l2_cache_str},
	{ 0x47, 8, 64, 8*1024*1024, l3_cache_str},
	{ 0x46, 4, 64, 4*1024*1024, l3_cache_str},
	{ 0x45, 4, 32, 2*1024*1024, l2_cache_str},
	{ 0x44, 4, 32, 1024*1024, l2_cache_str},
	{ 0x43, 4, 32, 512*1024, l2_cache_str},
	{ 0x42, 4, 32, 256*1024, l2_cache_str},
	{ 0x41, 4, 32, 128*1024, l2_cache_str},
	{ 0x3e, 4, 64, 512*1024, sl2_cache_str},
	{ 0x3d, 6, 64, 384*1024, sl2_cache_str},
	{ 0x3c, 4, 64, 256*1024, sl2_cache_str},
	{ 0x3b, 2, 64, 128*1024, sl2_cache_str},
	{ 0x3a, 6, 64, 192*1024, sl2_cache_str},
	{ 0x39, 4, 64, 128*1024, sl2_cache_str},
	{ 0x30, 8, 64, 32*1024, l1_icache_str},
	{ 0x2c, 8, 64, 32*1024, l1_dcache_str},
	{ 0x29, 8, 64, 4096*1024, sl3_cache_str},
	{ 0x25, 8, 64, 2048*1024, sl3_cache_str},
	{ 0x23, 8, 64, 1024*1024, sl3_cache_str},
	{ 0x22, 4, 64, 512*1024, sl3_cache_str},
	{ 0x0e, 6, 64, 24*1024, l1_dcache_str},
	{ 0x0d, 4, 32, 16*1024, l1_dcache_str},
	{ 0x0c, 4, 32, 16*1024, l1_dcache_str},
	{ 0x0b, 4, 0, 4, itlb4M_str},
	{ 0x0a, 2, 32, 8*1024, l1_dcache_str},
	{ 0x08, 4, 32, 16*1024, l1_icache_str},
	{ 0x06, 4, 32, 8*1024, l1_icache_str},
	{ 0x05, 4, 0, 32, dtlb4M_str},
	{ 0x04, 4, 0, 8, dtlb4M_str},
	{ 0x03, 4, 0, 64, dtlb4k_str},
	{ 0x02, 4, 0, 2, itlb4M_str},
	{ 0x01, 4, 0, 32, itlb4k_str},
	{ 0 }
};

static const struct cachetab cyrix_ctab[] = {
	{ 0x70, 4, 0, 32, "tlb-4K" },
	{ 0x80, 4, 16, 16*1024, "l1-cache" },
	{ 0 }
};

/*
 * Search a cache table for a matching entry
 */
static const struct cachetab *
find_cacheent(const struct cachetab *ct, uint_t code)
{
	if (code != 0) {
		for (; ct->ct_code != 0; ct++)
			if (ct->ct_code <= code)
				break;
		if (ct->ct_code == code)
			return (ct);
	}
	return (NULL);
}

/*
 * Populate cachetab entry with L2 or L3 cache-information using
 * cpuid function 4. This function is called from intel_walk_cacheinfo()
 * when descriptor 0x49 is encountered. It returns 0 if no such cache
 * information is found.
 */
static int
intel_cpuid_4_cache_info(struct cachetab *ct, struct cpuid_info *cpi)
{
	uint32_t level, i;
	int ret = 0;

	for (i = 0; i < cpi->cpi_cache_leaf_size; i++) {
		level = CPI_CACHE_LVL(cpi->cpi_cache_leaves[i]);

		if (level == 2 || level == 3) {
			ct->ct_assoc =
			    CPI_CACHE_WAYS(cpi->cpi_cache_leaves[i]) + 1;
			ct->ct_line_size =
			    CPI_CACHE_COH_LN_SZ(cpi->cpi_cache_leaves[i]) + 1;
			ct->ct_size = ct->ct_assoc *
			    (CPI_CACHE_PARTS(cpi->cpi_cache_leaves[i]) + 1) *
			    ct->ct_line_size *
			    (cpi->cpi_cache_leaves[i]->cp_ecx + 1);

			if (level == 2) {
				ct->ct_label = l2_cache_str;
			} else if (level == 3) {
				ct->ct_label = l3_cache_str;
			}
			ret = 1;
		}
	}

	return (ret);
}

/*
 * Walk the cacheinfo descriptor, applying 'func' to every valid element
 * The walk is terminated if the walker returns non-zero.
 */
static void
intel_walk_cacheinfo(struct cpuid_info *cpi,
    void *arg, int (*func)(void *, const struct cachetab *))
{
	const struct cachetab *ct;
	struct cachetab des_49_ct, des_b1_ct;
	uint8_t *dp;
	int i;

	if ((dp = cpi->cpi_cacheinfo) == NULL)
		return;
	for (i = 0; i < cpi->cpi_ncache; i++, dp++) {
		/*
		 * For overloaded descriptor 0x49 we use cpuid function 4
		 * if supported by the current processor, to create
		 * cache information.
		 * For overloaded descriptor 0xb1 we use X86_PAE flag
		 * to disambiguate the cache information.
		 */
		if (*dp == 0x49 && cpi->cpi_maxeax >= 0x4 &&
		    intel_cpuid_4_cache_info(&des_49_ct, cpi) == 1) {
				ct = &des_49_ct;
		} else if (*dp == 0xb1) {
			des_b1_ct.ct_code = 0xb1;
			des_b1_ct.ct_assoc = 4;
			des_b1_ct.ct_line_size = 0;
			if (is_x86_feature(x86_featureset, X86FSET_PAE)) {
				des_b1_ct.ct_size = 8;
				des_b1_ct.ct_label = itlb2M_str;
			} else {
				des_b1_ct.ct_size = 4;
				des_b1_ct.ct_label = itlb4M_str;
			}
			ct = &des_b1_ct;
		} else {
			if ((ct = find_cacheent(intel_ctab, *dp)) == NULL) {
				continue;
			}
		}

		if (func(arg, ct) != 0) {
			break;
		}
	}
}

/*
 * (Like the Intel one, except for Cyrix CPUs)
 */
static void
cyrix_walk_cacheinfo(struct cpuid_info *cpi,
    void *arg, int (*func)(void *, const struct cachetab *))
{
	const struct cachetab *ct;
	uint8_t *dp;
	int i;

	if ((dp = cpi->cpi_cacheinfo) == NULL)
		return;
	for (i = 0; i < cpi->cpi_ncache; i++, dp++) {
		/*
		 * Search Cyrix-specific descriptor table first ..
		 */
		if ((ct = find_cacheent(cyrix_ctab, *dp)) != NULL) {
			if (func(arg, ct) != 0)
				break;
			continue;
		}
		/*
		 * .. else fall back to the Intel one
		 */
		if ((ct = find_cacheent(intel_ctab, *dp)) != NULL) {
			if (func(arg, ct) != 0)
				break;
			continue;
		}
	}
}

/*
 * A cacheinfo walker that adds associativity, line-size, and size properties
 * to the devinfo node it is passed as an argument.
 */
static int
add_cacheent_props(void *arg, const struct cachetab *ct)
{
	dev_info_t *devi = arg;

	add_cache_prop(devi, ct->ct_label, assoc_str, ct->ct_assoc);
	if (ct->ct_line_size != 0)
		add_cache_prop(devi, ct->ct_label, line_str,
		    ct->ct_line_size);
	add_cache_prop(devi, ct->ct_label, size_str, ct->ct_size);
	return (0);
}


static const char fully_assoc[] = "fully-associative?";

/*
 * AMD style cache/tlb description
 *
 * Extended functions 5 and 6 directly describe properties of
 * tlbs and various cache levels.
 */
static void
add_amd_assoc(dev_info_t *devi, const char *label, uint_t assoc)
{
	switch (assoc) {
	case 0:	/* reserved; ignore */
		break;
	default:
		add_cache_prop(devi, label, assoc_str, assoc);
		break;
	case 0xff:
		add_cache_prop(devi, label, fully_assoc, 1);
		break;
	}
}

static void
add_amd_tlb(dev_info_t *devi, const char *label, uint_t assoc, uint_t size)
{
	if (size == 0)
		return;
	add_cache_prop(devi, label, size_str, size);
	add_amd_assoc(devi, label, assoc);
}

static void
add_amd_cache(dev_info_t *devi, const char *label,
    uint_t size, uint_t assoc, uint_t lines_per_tag, uint_t line_size)
{
	if (size == 0 || line_size == 0)
		return;
	add_amd_assoc(devi, label, assoc);
	/*
	 * Most AMD parts have a sectored cache. Multiple cache lines are
	 * associated with each tag. A sector consists of all cache lines
	 * associated with a tag. For example, the AMD K6-III has a sector
	 * size of 2 cache lines per tag.
	 */
	if (lines_per_tag != 0)
		add_cache_prop(devi, label, "lines-per-tag", lines_per_tag);
	add_cache_prop(devi, label, line_str, line_size);
	add_cache_prop(devi, label, size_str, size * 1024);
}

static void
add_amd_l2_assoc(dev_info_t *devi, const char *label, uint_t assoc)
{
	switch (assoc) {
	case 0:	/* off */
		break;
	case 1:
	case 2:
	case 4:
		add_cache_prop(devi, label, assoc_str, assoc);
		break;
	case 6:
		add_cache_prop(devi, label, assoc_str, 8);
		break;
	case 8:
		add_cache_prop(devi, label, assoc_str, 16);
		break;
	case 0xf:
		add_cache_prop(devi, label, fully_assoc, 1);
		break;
	default: /* reserved; ignore */
		break;
	}
}

static void
add_amd_l2_tlb(dev_info_t *devi, const char *label, uint_t assoc, uint_t size)
{
	if (size == 0 || assoc == 0)
		return;
	add_amd_l2_assoc(devi, label, assoc);
	add_cache_prop(devi, label, size_str, size);
}

static void
add_amd_l2_cache(dev_info_t *devi, const char *label,
    uint_t size, uint_t assoc, uint_t lines_per_tag, uint_t line_size)
{
	if (size == 0 || assoc == 0 || line_size == 0)
		return;
	add_amd_l2_assoc(devi, label, assoc);
	if (lines_per_tag != 0)
		add_cache_prop(devi, label, "lines-per-tag", lines_per_tag);
	add_cache_prop(devi, label, line_str, line_size);
	add_cache_prop(devi, label, size_str, size * 1024);
}

static void
amd_cache_info(struct cpuid_info *cpi, dev_info_t *devi)
{
	struct cpuid_regs *cp;

	if (cpi->cpi_xmaxeax < 0x80000005)
		return;
	cp = &cpi->cpi_extd[5];

	/*
	 * 4M/2M L1 TLB configuration
	 *
	 * We report the size for 2M pages because AMD uses two
	 * TLB entries for one 4M page.
	 */
	add_amd_tlb(devi, "dtlb-2M",
	    BITX(cp->cp_eax, 31, 24), BITX(cp->cp_eax, 23, 16));
	add_amd_tlb(devi, "itlb-2M",
	    BITX(cp->cp_eax, 15, 8), BITX(cp->cp_eax, 7, 0));

	/*
	 * 4K L1 TLB configuration
	 */

	switch (cpi->cpi_vendor) {
		uint_t nentries;
	case X86_VENDOR_TM:
		if (cpi->cpi_family >= 5) {
			/*
			 * Crusoe processors have 256 TLB entries, but
			 * cpuid data format constrains them to only
			 * reporting 255 of them.
			 */
			if ((nentries = BITX(cp->cp_ebx, 23, 16)) == 255)
				nentries = 256;
			/*
			 * Crusoe processors also have a unified TLB
			 */
			add_amd_tlb(devi, "tlb-4K", BITX(cp->cp_ebx, 31, 24),
			    nentries);
			break;
		}
		/*FALLTHROUGH*/
	default:
		add_amd_tlb(devi, itlb4k_str,
		    BITX(cp->cp_ebx, 31, 24), BITX(cp->cp_ebx, 23, 16));
		add_amd_tlb(devi, dtlb4k_str,
		    BITX(cp->cp_ebx, 15, 8), BITX(cp->cp_ebx, 7, 0));
		break;
	}

	/*
	 * data L1 cache configuration
	 */

	add_amd_cache(devi, l1_dcache_str,
	    BITX(cp->cp_ecx, 31, 24), BITX(cp->cp_ecx, 23, 16),
	    BITX(cp->cp_ecx, 15, 8), BITX(cp->cp_ecx, 7, 0));

	/*
	 * code L1 cache configuration
	 */

	add_amd_cache(devi, l1_icache_str,
	    BITX(cp->cp_edx, 31, 24), BITX(cp->cp_edx, 23, 16),
	    BITX(cp->cp_edx, 15, 8), BITX(cp->cp_edx, 7, 0));

	if (cpi->cpi_xmaxeax < 0x80000006)
		return;
	cp = &cpi->cpi_extd[6];

	/* Check for a unified L2 TLB for large pages */

	if (BITX(cp->cp_eax, 31, 16) == 0)
		add_amd_l2_tlb(devi, "l2-tlb-2M",
		    BITX(cp->cp_eax, 15, 12), BITX(cp->cp_eax, 11, 0));
	else {
		add_amd_l2_tlb(devi, "l2-dtlb-2M",
		    BITX(cp->cp_eax, 31, 28), BITX(cp->cp_eax, 27, 16));
		add_amd_l2_tlb(devi, "l2-itlb-2M",
		    BITX(cp->cp_eax, 15, 12), BITX(cp->cp_eax, 11, 0));
	}

	/* Check for a unified L2 TLB for 4K pages */

	if (BITX(cp->cp_ebx, 31, 16) == 0) {
		add_amd_l2_tlb(devi, "l2-tlb-4K",
		    BITX(cp->cp_eax, 15, 12), BITX(cp->cp_eax, 11, 0));
	} else {
		add_amd_l2_tlb(devi, "l2-dtlb-4K",
		    BITX(cp->cp_eax, 31, 28), BITX(cp->cp_eax, 27, 16));
		add_amd_l2_tlb(devi, "l2-itlb-4K",
		    BITX(cp->cp_eax, 15, 12), BITX(cp->cp_eax, 11, 0));
	}

	add_amd_l2_cache(devi, l2_cache_str,
	    BITX(cp->cp_ecx, 31, 16), BITX(cp->cp_ecx, 15, 12),
	    BITX(cp->cp_ecx, 11, 8), BITX(cp->cp_ecx, 7, 0));
}

/*
 * There are two basic ways that the x86 world describes it cache
 * and tlb architecture - Intel's way and AMD's way.
 *
 * Return which flavor of cache architecture we should use
 */
static int
x86_which_cacheinfo(struct cpuid_info *cpi)
{
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		if (cpi->cpi_maxeax >= 2)
			return (X86_VENDOR_Intel);
		break;
	case X86_VENDOR_AMD:
		/*
		 * The K5 model 1 was the first part from AMD that reported
		 * cache sizes via extended cpuid functions.
		 */
		if (cpi->cpi_family > 5 ||
		    (cpi->cpi_family == 5 && cpi->cpi_model >= 1))
			return (X86_VENDOR_AMD);
		break;
	case X86_VENDOR_TM:
		if (cpi->cpi_family >= 5)
			return (X86_VENDOR_AMD);
		/*FALLTHROUGH*/
	default:
		/*
		 * If they have extended CPU data for 0x80000005
		 * then we assume they have AMD-format cache
		 * information.
		 *
		 * If not, and the vendor happens to be Cyrix,
		 * then try our-Cyrix specific handler.
		 *
		 * If we're not Cyrix, then assume we're using Intel's
		 * table-driven format instead.
		 */
		if (cpi->cpi_xmaxeax >= 0x80000005)
			return (X86_VENDOR_AMD);
		else if (cpi->cpi_vendor == X86_VENDOR_Cyrix)
			return (X86_VENDOR_Cyrix);
		else if (cpi->cpi_maxeax >= 2)
			return (X86_VENDOR_Intel);
		break;
	}
	return (-1);
}

void
cpuid_set_cpu_properties(void *dip, processorid_t cpu_id,
    struct cpuid_info *cpi)
{
	dev_info_t *cpu_devi;
	int create;

	cpu_devi = (dev_info_t *)dip;

	/* device_type */
	(void) ndi_prop_update_string(DDI_DEV_T_NONE, cpu_devi,
	    "device_type", "cpu");

	/* reg */
	(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
	    "reg", cpu_id);

	/* cpu-mhz, and clock-frequency */
	if (cpu_freq > 0) {
		long long mul;

		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "cpu-mhz", cpu_freq);
		if ((mul = cpu_freq * 1000000LL) <= INT_MAX)
			(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
			    "clock-frequency", (int)mul);
	}

	if (!is_x86_feature(x86_featureset, X86FSET_CPUID)) {
		return;
	}

	/* vendor-id */
	(void) ndi_prop_update_string(DDI_DEV_T_NONE, cpu_devi,
	    "vendor-id", cpi->cpi_vendorstr);

	if (cpi->cpi_maxeax == 0) {
		return;
	}

	/*
	 * family, model, and step
	 */
	(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
	    "family", CPI_FAMILY(cpi));
	(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
	    "cpu-model", CPI_MODEL(cpi));
	(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
	    "stepping-id", CPI_STEP(cpi));

	/* type */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		create = 1;
		break;
	default:
		create = 0;
		break;
	}
	if (create)
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "type", CPI_TYPE(cpi));

	/* ext-family */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
	case X86_VENDOR_AMD:
		create = cpi->cpi_family >= 0xf;
		break;
	default:
		create = 0;
		break;
	}
	if (create)
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "ext-family", CPI_FAMILY_XTD(cpi));

	/* ext-model */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		create = IS_EXTENDED_MODEL_INTEL(cpi);
		break;
	case X86_VENDOR_AMD:
		create = CPI_FAMILY(cpi) == 0xf;
		break;
	default:
		create = 0;
		break;
	}
	if (create)
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "ext-model", CPI_MODEL_XTD(cpi));

	/* generation */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_AMD:
		/*
		 * AMD K5 model 1 was the first part to support this
		 */
		create = cpi->cpi_xmaxeax >= 0x80000001;
		break;
	default:
		create = 0;
		break;
	}
	if (create)
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "generation", BITX((cpi)->cpi_extd[1].cp_eax, 11, 8));

	/* brand-id */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		/*
		 * brand id first appeared on Pentium III Xeon model 8,
		 * and Celeron model 8 processors and Opteron
		 */
		create = cpi->cpi_family > 6 ||
		    (cpi->cpi_family == 6 && cpi->cpi_model >= 8);
		break;
	case X86_VENDOR_AMD:
		create = cpi->cpi_family >= 0xf;
		break;
	default:
		create = 0;
		break;
	}
	if (create && cpi->cpi_brandid != 0) {
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "brand-id", cpi->cpi_brandid);
	}

	/* chunks, and apic-id */
	switch (cpi->cpi_vendor) {
		/*
		 * first available on Pentium IV and Opteron (K8)
		 */
	case X86_VENDOR_Intel:
		create = IS_NEW_F6(cpi) || cpi->cpi_family >= 0xf;
		break;
	case X86_VENDOR_AMD:
		create = cpi->cpi_family >= 0xf;
		break;
	default:
		create = 0;
		break;
	}
	if (create) {
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "chunks", CPI_CHUNKS(cpi));
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "apic-id", cpi->cpi_apicid);
		if (cpi->cpi_chipid >= 0) {
			(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
			    "chip#", cpi->cpi_chipid);
			(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
			    "clog#", cpi->cpi_clogid);
		}
	}

	/* cpuid-features */
	(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
	    "cpuid-features", CPI_FEATURES_EDX(cpi));


	/* cpuid-features-ecx */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		create = IS_NEW_F6(cpi) || cpi->cpi_family >= 0xf;
		break;
	case X86_VENDOR_AMD:
		create = cpi->cpi_family >= 0xf;
		break;
	default:
		create = 0;
		break;
	}
	if (create)
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "cpuid-features-ecx", CPI_FEATURES_ECX(cpi));

	/* ext-cpuid-features */
	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
	case X86_VENDOR_AMD:
	case X86_VENDOR_Cyrix:
	case X86_VENDOR_TM:
	case X86_VENDOR_Centaur:
		create = cpi->cpi_xmaxeax >= 0x80000001;
		break;
	default:
		create = 0;
		break;
	}
	if (create) {
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "ext-cpuid-features", CPI_FEATURES_XTD_EDX(cpi));
		(void) ndi_prop_update_int(DDI_DEV_T_NONE, cpu_devi,
		    "ext-cpuid-features-ecx", CPI_FEATURES_XTD_ECX(cpi));
	}

	/*
	 * Brand String first appeared in Intel Pentium IV, AMD K5
	 * model 1, and Cyrix GXm.  On earlier models we try and
	 * simulate something similar .. so this string should always
	 * same -something- about the processor, however lame.
	 */
	(void) ndi_prop_update_string(DDI_DEV_T_NONE, cpu_devi,
	    "brand-string", cpi->cpi_brandstr);

	/*
	 * Finally, cache and tlb information
	 */
	switch (x86_which_cacheinfo(cpi)) {
	case X86_VENDOR_Intel:
		intel_walk_cacheinfo(cpi, cpu_devi, add_cacheent_props);
		break;
	case X86_VENDOR_Cyrix:
		cyrix_walk_cacheinfo(cpi, cpu_devi, add_cacheent_props);
		break;
	case X86_VENDOR_AMD:
		amd_cache_info(cpi, cpu_devi);
		break;
	default:
		break;
	}
}

struct l2info {
	int *l2i_csz;
	int *l2i_lsz;
	int *l2i_assoc;
	int l2i_ret;
};

/*
 * A cacheinfo walker that fetches the size, line-size and associativity
 * of the L2 cache
 */
static int
intel_l2cinfo(void *arg, const struct cachetab *ct)
{
	struct l2info *l2i = arg;
	int *ip;

	if (ct->ct_label != l2_cache_str &&
	    ct->ct_label != sl2_cache_str)
		return (0);	/* not an L2 -- keep walking */

	if ((ip = l2i->l2i_csz) != NULL)
		*ip = ct->ct_size;
	if ((ip = l2i->l2i_lsz) != NULL)
		*ip = ct->ct_line_size;
	if ((ip = l2i->l2i_assoc) != NULL)
		*ip = ct->ct_assoc;
	l2i->l2i_ret = ct->ct_size;
	return (1);		/* was an L2 -- terminate walk */
}

/*
 * AMD L2/L3 Cache and TLB Associativity Field Definition:
 *
 *	Unlike the associativity for the L1 cache and tlb where the 8 bit
 *	value is the associativity, the associativity for the L2 cache and
 *	tlb is encoded in the following table. The 4 bit L2 value serves as
 *	an index into the amd_afd[] array to determine the associativity.
 *	-1 is undefined. 0 is fully associative.
 */

static int amd_afd[] =
	{-1, 1, 2, -1, 4, -1, 8, -1, 16, -1, 32, 48, 64, 96, 128, 0};

static void
amd_l2cacheinfo(struct cpuid_info *cpi, struct l2info *l2i)
{
	struct cpuid_regs *cp;
	uint_t size, assoc;
	int i;
	int *ip;

	if (cpi->cpi_xmaxeax < 0x80000006)
		return;
	cp = &cpi->cpi_extd[6];

	if ((i = BITX(cp->cp_ecx, 15, 12)) != 0 &&
	    (size = BITX(cp->cp_ecx, 31, 16)) != 0) {
		uint_t cachesz = size * 1024;
		assoc = amd_afd[i];

		ASSERT(assoc != -1);

		if ((ip = l2i->l2i_csz) != NULL)
			*ip = cachesz;
		if ((ip = l2i->l2i_lsz) != NULL)
			*ip = BITX(cp->cp_ecx, 7, 0);
		if ((ip = l2i->l2i_assoc) != NULL)
			*ip = assoc;
		l2i->l2i_ret = cachesz;
	}
}

int
getl2cacheinfo(cpu_t *cpu, int *csz, int *lsz, int *assoc)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	struct l2info __l2info, *l2i = &__l2info;

	l2i->l2i_csz = csz;
	l2i->l2i_lsz = lsz;
	l2i->l2i_assoc = assoc;
	l2i->l2i_ret = -1;

	switch (x86_which_cacheinfo(cpi)) {
	case X86_VENDOR_Intel:
		intel_walk_cacheinfo(cpi, l2i, intel_l2cinfo);
		break;
	case X86_VENDOR_Cyrix:
		cyrix_walk_cacheinfo(cpi, l2i, intel_l2cinfo);
		break;
	case X86_VENDOR_AMD:
		amd_l2cacheinfo(cpi, l2i);
		break;
	default:
		break;
	}
	return (l2i->l2i_ret);
}

#if !defined(__xpv)

uint32_t *
cpuid_mwait_alloc(cpu_t *cpu)
{
	uint32_t	*ret;
	size_t		mwait_size;

	ASSERT(cpuid_checkpass(CPU, 2));

	mwait_size = CPU->cpu_m.mcpu_cpi->cpi_mwait.mon_max;
	if (mwait_size == 0)
		return (NULL);

	/*
	 * kmem_alloc() returns cache line size aligned data for mwait_size
	 * allocations.  mwait_size is currently cache line sized.  Neither
	 * of these implementation details are guarantied to be true in the
	 * future.
	 *
	 * First try allocating mwait_size as kmem_alloc() currently returns
	 * correctly aligned memory.  If kmem_alloc() does not return
	 * mwait_size aligned memory, then use mwait_size ROUNDUP.
	 *
	 * Set cpi_mwait.buf_actual and cpi_mwait.size_actual in case we
	 * decide to free this memory.
	 */
	ret = kmem_zalloc(mwait_size, KM_SLEEP);
	if (ret == (uint32_t *)P2ROUNDUP((uintptr_t)ret, mwait_size)) {
		cpu->cpu_m.mcpu_cpi->cpi_mwait.buf_actual = ret;
		cpu->cpu_m.mcpu_cpi->cpi_mwait.size_actual = mwait_size;
		*ret = MWAIT_RUNNING;
		return (ret);
	} else {
		kmem_free(ret, mwait_size);
		ret = kmem_zalloc(mwait_size * 2, KM_SLEEP);
		cpu->cpu_m.mcpu_cpi->cpi_mwait.buf_actual = ret;
		cpu->cpu_m.mcpu_cpi->cpi_mwait.size_actual = mwait_size * 2;
		ret = (uint32_t *)P2ROUNDUP((uintptr_t)ret, mwait_size);
		*ret = MWAIT_RUNNING;
		return (ret);
	}
}

void
cpuid_mwait_free(cpu_t *cpu)
{
	if (cpu->cpu_m.mcpu_cpi == NULL) {
		return;
	}

	if (cpu->cpu_m.mcpu_cpi->cpi_mwait.buf_actual != NULL &&
	    cpu->cpu_m.mcpu_cpi->cpi_mwait.size_actual > 0) {
		kmem_free(cpu->cpu_m.mcpu_cpi->cpi_mwait.buf_actual,
		    cpu->cpu_m.mcpu_cpi->cpi_mwait.size_actual);
	}

	cpu->cpu_m.mcpu_cpi->cpi_mwait.buf_actual = NULL;
	cpu->cpu_m.mcpu_cpi->cpi_mwait.size_actual = 0;
}

void
patch_tsc_read(int flag)
{
	size_t cnt;

	switch (flag) {
	case TSC_NONE:
		cnt = &_no_rdtsc_end - &_no_rdtsc_start;
		(void) memcpy((void *)tsc_read, (void *)&_no_rdtsc_start, cnt);
		break;
	case TSC_RDTSC_MFENCE:
		cnt = &_tsc_mfence_end - &_tsc_mfence_start;
		(void) memcpy((void *)tsc_read,
		    (void *)&_tsc_mfence_start, cnt);
		break;
	case TSC_RDTSC_LFENCE:
		cnt = &_tsc_lfence_end - &_tsc_lfence_start;
		(void) memcpy((void *)tsc_read,
		    (void *)&_tsc_lfence_start, cnt);
		break;
	case TSC_TSCP:
		cnt = &_tscp_end - &_tscp_start;
		(void) memcpy((void *)tsc_read, (void *)&_tscp_start, cnt);
		break;
	default:
		/* Bail for unexpected TSC types. (TSC_NONE covers 0) */
		cmn_err(CE_PANIC, "Unrecogized TSC type: %d", flag);
		break;
	}
	tsc_type = flag;
}

int
cpuid_deep_cstates_supported(void)
{
	struct cpuid_info *cpi;
	struct cpuid_regs regs;

	ASSERT(cpuid_checkpass(CPU, 1));

	cpi = CPU->cpu_m.mcpu_cpi;

	if (!is_x86_feature(x86_featureset, X86FSET_CPUID))
		return (0);

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		if (cpi->cpi_xmaxeax < 0x80000007)
			return (0);

		/*
		 * TSC run at a constant rate in all ACPI C-states?
		 */
		regs.cp_eax = 0x80000007;
		(void) __cpuid_insn(&regs);
		return (regs.cp_edx & CPUID_TSC_CSTATE_INVARIANCE);

	default:
		return (0);
	}
}

#endif	/* !__xpv */

void
post_startup_cpu_fixups(void)
{
#ifndef __xpv
	/*
	 * Some AMD processors support C1E state. Entering this state will
	 * cause the local APIC timer to stop, which we can't deal with at
	 * this time.
	 */
	if (cpuid_getvendor(CPU) == X86_VENDOR_AMD) {
		on_trap_data_t otd;
		uint64_t reg;

		if (!on_trap(&otd, OT_DATA_ACCESS)) {
			reg = rdmsr(MSR_AMD_INT_PENDING_CMP_HALT);
			/* Disable C1E state if it is enabled by BIOS */
			if ((reg >> AMD_ACTONCMPHALT_SHIFT) &
			    AMD_ACTONCMPHALT_MASK) {
				reg &= ~(AMD_ACTONCMPHALT_MASK <<
				    AMD_ACTONCMPHALT_SHIFT);
				wrmsr(MSR_AMD_INT_PENDING_CMP_HALT, reg);
			}
		}
		no_trap();
	}
#endif	/* !__xpv */
}

void
enable_pcid(void)
{
	if (x86_use_pcid == -1)
		x86_use_pcid = is_x86_feature(x86_featureset, X86FSET_PCID);

	if (x86_use_invpcid == -1) {
		x86_use_invpcid = is_x86_feature(x86_featureset,
		    X86FSET_INVPCID);
	}

	if (!x86_use_pcid)
		return;

	/*
	 * Intel say that on setting PCIDE, it immediately starts using the PCID
	 * bits; better make sure there's nothing there.
	 */
	ASSERT((getcr3() & MMU_PAGEOFFSET) == PCID_NONE);

	setcr4(getcr4() | CR4_PCIDE);
}

/*
 * Setup necessary registers to enable XSAVE feature on this processor.
 * This function needs to be called early enough, so that no xsave/xrstor
 * ops will execute on the processor before the MSRs are properly set up.
 *
 * Current implementation has the following assumption:
 * - cpuid_pass1() is done, so that X86 features are known.
 * - fpu_probe() is done, so that fp_save_mech is chosen.
 */
void
xsave_setup_msr(cpu_t *cpu)
{
	ASSERT(fp_save_mech == FP_XSAVE);
	ASSERT(is_x86_feature(x86_featureset, X86FSET_XSAVE));

	/* Enable OSXSAVE in CR4. */
	setcr4(getcr4() | CR4_OSXSAVE);
	/*
	 * Update SW copy of ECX, so that /dev/cpu/self/cpuid will report
	 * correct value.
	 */
	cpu->cpu_m.mcpu_cpi->cpi_std[1].cp_ecx |= CPUID_INTC_ECX_OSXSAVE;
	setup_xfem();
}

/*
 * Starting with the Westmere processor the local
 * APIC timer will continue running in all C-states,
 * including the deepest C-states.
 */
int
cpuid_arat_supported(void)
{
	struct cpuid_info *cpi;
	struct cpuid_regs regs;

	ASSERT(cpuid_checkpass(CPU, 1));
	ASSERT(is_x86_feature(x86_featureset, X86FSET_CPUID));

	cpi = CPU->cpu_m.mcpu_cpi;

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		/*
		 * Always-running Local APIC Timer is
		 * indicated by CPUID.6.EAX[2].
		 */
		if (cpi->cpi_maxeax >= 6) {
			regs.cp_eax = 6;
			(void) cpuid_insn(NULL, &regs);
			return (regs.cp_eax & CPUID_INTC_EAX_ARAT);
		} else {
			return (0);
		}
	default:
		return (0);
	}
}

/*
 * Check support for Intel ENERGY_PERF_BIAS feature
 */
int
cpuid_iepb_supported(struct cpu *cp)
{
	struct cpuid_info *cpi = cp->cpu_m.mcpu_cpi;
	struct cpuid_regs regs;

	ASSERT(cpuid_checkpass(cp, 1));

	if (!(is_x86_feature(x86_featureset, X86FSET_CPUID)) ||
	    !(is_x86_feature(x86_featureset, X86FSET_MSR))) {
		return (0);
	}

	/*
	 * Intel ENERGY_PERF_BIAS MSR is indicated by
	 * capability bit CPUID.6.ECX.3
	 */
	if ((cpi->cpi_vendor != X86_VENDOR_Intel) || (cpi->cpi_maxeax < 6))
		return (0);

	regs.cp_eax = 0x6;
	(void) cpuid_insn(NULL, &regs);
	return (regs.cp_ecx & CPUID_INTC_ECX_PERFBIAS);
}

/*
 * Check support for TSC deadline timer
 *
 * TSC deadline timer provides a superior software programming
 * model over local APIC timer that eliminates "time drifts".
 * Instead of specifying a relative time, software specifies an
 * absolute time as the target at which the processor should
 * generate a timer event.
 */
int
cpuid_deadline_tsc_supported(void)
{
	struct cpuid_info *cpi = CPU->cpu_m.mcpu_cpi;
	struct cpuid_regs regs;

	ASSERT(cpuid_checkpass(CPU, 1));
	ASSERT(is_x86_feature(x86_featureset, X86FSET_CPUID));

	switch (cpi->cpi_vendor) {
	case X86_VENDOR_Intel:
		if (cpi->cpi_maxeax >= 1) {
			regs.cp_eax = 1;
			(void) cpuid_insn(NULL, &regs);
			return (regs.cp_ecx & CPUID_DEADLINE_TSC);
		} else {
			return (0);
		}
	default:
		return (0);
	}
}

#if defined(__amd64) && !defined(__xpv)
/*
 * Patch in versions of bcopy for high performance Intel Nhm processors
 * and later...
 */
void
patch_memops(uint_t vendor)
{
	size_t cnt, i;
	caddr_t to, from;

	if ((vendor == X86_VENDOR_Intel) &&
	    is_x86_feature(x86_featureset, X86FSET_SSE4_2)) {
		cnt = &bcopy_patch_end - &bcopy_patch_start;
		to = &bcopy_ck_size;
		from = &bcopy_patch_start;
		for (i = 0; i < cnt; i++) {
			*to++ = *from++;
		}
	}
}
#endif  /* __amd64 && !__xpv */

/*
 * We're being asked to tell the system how many bits are required to represent
 * the various thread and strand IDs. While it's tempting to derive this based
 * on the values in cpi_ncore_per_chip and cpi_ncpu_per_chip, that isn't quite
 * correct. Instead, this needs to be based on the number of bits that the APIC
 * allows for these different configurations. We only update these to a larger
 * value if we find one.
 */
void
cpuid_get_ext_topo(cpu_t *cpu, uint_t *core_nbits, uint_t *strand_nbits)
{
	struct cpuid_info *cpi;

	VERIFY(cpuid_checkpass(CPU, 1));
	cpi = cpu->cpu_m.mcpu_cpi;

	if (cpi->cpi_ncore_bits > *core_nbits) {
		*core_nbits = cpi->cpi_ncore_bits;
	}

	if (cpi->cpi_nthread_bits > *strand_nbits) {
		*strand_nbits = cpi->cpi_nthread_bits;
	}
}

void
cpuid_pass_ucode(cpu_t *cpu, uchar_t *fset)
{
	struct cpuid_info *cpi = cpu->cpu_m.mcpu_cpi;
	struct cpuid_regs cp;

	/*
	 * Reread the CPUID portions that we need for various security
	 * information.
	 */
	if (cpi->cpi_vendor == X86_VENDOR_Intel) {
		/*
		 * Check if we now have leaf 7 available to us.
		 */
		if (cpi->cpi_maxeax < 7) {
			bzero(&cp, sizeof (cp));
			cp.cp_eax = 0;
			cpi->cpi_maxeax = __cpuid_insn(&cp);
			if (cpi->cpi_maxeax < 7)
				return;
		}

		bzero(&cp, sizeof (cp));
		cp.cp_eax = 7;
		cp.cp_ecx = 0;
		(void) __cpuid_insn(&cp);
		cpi->cpi_std[7] = cp;
	} else if (cpi->cpi_vendor == X86_VENDOR_AMD) {
		/* No xcpuid support */
		if (cpi->cpi_family < 5 ||
		    (cpi->cpi_family == 5 && cpi->cpi_model < 1))
			return;

		if (cpi->cpi_xmaxeax < CPUID_LEAF_EXT_8) {
			bzero(&cp, sizeof (cp));
			cp.cp_eax = CPUID_LEAF_EXT_0;
			cpi->cpi_xmaxeax = __cpuid_insn(&cp);
			if (cpi->cpi_xmaxeax < CPUID_LEAF_EXT_8) {
				return;
			}
		}

		bzero(&cp, sizeof (cp));
		cp.cp_eax = CPUID_LEAF_EXT_8;
		(void) __cpuid_insn(&cp);
		platform_cpuid_mangle(cpi->cpi_vendor, CPUID_LEAF_EXT_8, &cp);
		cpi->cpi_extd[8] = cp;
	} else {
		/*
		 * Nothing to do here. Return an empty set which has already
		 * been zeroed for us.
		 */
		return;
	}
	cpuid_scan_security(cpu, fset);
}

/* ARGSUSED */
static int
cpuid_post_ucodeadm_xc(xc_arg_t arg0, xc_arg_t arg1, xc_arg_t arg2)
{
	uchar_t *fset;
	boolean_t first_pass = (boolean_t)arg1;

	fset = (uchar_t *)(arg0 + sizeof (x86_featureset) * CPU->cpu_id);
	if (first_pass && CPU->cpu_id != 0)
		return (0);
	if (!first_pass && CPU->cpu_id == 0)
		return (0);
	cpuid_pass_ucode(CPU, fset);

	return (0);
}

/*
 * After a microcode update where the version has changed, then we need to
 * rescan CPUID. To do this we check every CPU to make sure that they have the
 * same microcode. Then we perform a cross call to all such CPUs. It's the
 * caller's job to make sure that no one else can end up doing an update while
 * this is going on.
 *
 * We assume that the system is microcode capable if we're called.
 */
void
cpuid_post_ucodeadm(void)
{
	uint32_t rev;
	int i;
	struct cpu *cpu;
	cpuset_t cpuset;
	void *argdata;
	uchar_t *f0;

	argdata = kmem_zalloc(sizeof (x86_featureset) * NCPU, KM_SLEEP);

	mutex_enter(&cpu_lock);
	cpu = cpu_get(0);
	rev = cpu->cpu_m.mcpu_ucode_info->cui_rev;
	CPUSET_ONLY(cpuset, 0);
	for (i = 1; i < max_ncpus; i++) {
		if ((cpu = cpu_get(i)) == NULL)
			continue;

		if (cpu->cpu_m.mcpu_ucode_info->cui_rev != rev) {
			panic("post microcode update CPU %d has differing "
			    "microcode revision (%u) from CPU 0 (%u)",
			    i, cpu->cpu_m.mcpu_ucode_info->cui_rev, rev);
		}
		CPUSET_ADD(cpuset, i);
	}

	/*
	 * We do the cross calls in two passes. The first pass is only for the
	 * boot CPU. The second pass is for all of the other CPUs. This allows
	 * the boot CPU to go through and change behavior related to patching or
	 * whether or not Enhanced IBRS needs to be enabled and then allow all
	 * other CPUs to follow suit.
	 */
	kpreempt_disable();
	xc_sync((xc_arg_t)argdata, B_TRUE, 0, CPUSET2BV(cpuset),
	    cpuid_post_ucodeadm_xc);
	xc_sync((xc_arg_t)argdata, B_FALSE, 0, CPUSET2BV(cpuset),
	    cpuid_post_ucodeadm_xc);
	kpreempt_enable();

	/*
	 * OK, now look at each CPU and see if their feature sets are equal.
	 */
	f0 = argdata;
	for (i = 1; i < max_ncpus; i++) {
		uchar_t *fset;
		if (!CPU_IN_SET(cpuset, i))
			continue;

		fset = (uchar_t *)((uintptr_t)argdata +
		    sizeof (x86_featureset) * i);

		if (!compare_x86_featureset(f0, fset)) {
			panic("Post microcode update CPU %d has "
			    "differing security feature (%p) set from CPU 0 "
			    "(%p), not appending to feature set", i,
			    (void *)fset, (void *)f0);
		}
	}

	mutex_exit(&cpu_lock);

	for (i = 0; i < NUM_X86_FEATURES; i++) {
		cmn_err(CE_CONT, "?post-ucode x86_feature: %s\n",
		    x86_feature_names[i]);
		if (is_x86_feature(f0, i)) {
			add_x86_feature(x86_featureset, i);
		}
	}
	kmem_free(argdata, sizeof (x86_featureset) * NCPU);
}
