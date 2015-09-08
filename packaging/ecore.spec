Name:       ecore
Summary:    Enlightened Core X interface library
Version:    1.7.1+svn.77580+build125
Release:    2
Group:      System/Libraries
License:    BSD 2-Clause
URL:        http://www.enlightenment.org
Source0:    %{name}-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  gettext
BuildRequires:  eina-devel
BuildRequires:  eet-devel
BuildRequires:  evas-devel
BuildRequires:  glib2-devel
BuildRequires:  libxcursor-devel
BuildRequires:  libxrender-devel
BuildRequires:  libxinerama-devel
BuildRequires:  libxrandr-devel
BuildRequires:  libxext-devel
BuildRequires:  libxi-devel
BuildRequires:  libxfixes-devel
BuildRequires:  libxcomposite-devel
BuildRequires:  libxdamage-devel
BuildRequires:  x11proto-xext
BuildRequires:  libxtst-devel
BuildRequires:  curl-devel
BuildRequires:  libjpeg-devel
BuildRequires:  libXgesture-devel


%description
Core abstraction layer for enlightenment DR 0.17 This is the core event abstraction layer and X abstraction layer that makes
 doing selections, Xdnd, general X stuff, and event loops, timeouts and idle
 handlers fast, optimized, and convenient. It's a separate library so anyone
 can make use of the work put into Ecore to make this job easy for
 applications.



%package devel
Summary:    Enlightened Core X interface library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Core abstraction layer for enlightenment (devel)

%package tools
Summary:    Enlightened Core X interface library (bin)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Provides:   %{name}-bin
Obsoletes:  %{name}-bin

%description tools
Core abstraction layer for enlightenment (tools)

%package con
Summary:    Enlightened Core X interface library (con)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description con
Core abstraction layer for enlightenment (con)

%package evas
Summary:    Enlightened Core X interface library (evas)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description evas
Core abstraction layer for enlightenment (evas)

%package file
Summary:    Enlightened Core X interface library (file)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description file
Core abstraction layer for enlightenment (file)

%package imf
Summary:    Enlightened Core X interface library (imf)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description imf
Core abstraction layer for enlightenment (imf)

%package imf-evas
Summary:    Enlightened Core X interface library (imf-evas)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description imf-evas
Core abstraction layer for enlightenment (imf-evas)

%package input
Summary:    Enlightened Core X interface library (input)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description input
Core abstraction layer for enlightenment (input)

%package input-evas
Summary:    Enlightened Core X interface library (input-evas)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description input-evas
Core abstraction layer for enlightenment (input-evas)

%package ipc
Summary:    Enlightened Core X interface library (ipc)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description ipc
Core abstraction layer for enlightenment (ipc)

%package x
Summary:    Enlightened Core X interface library (x)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description x
Core abstraction layer for enlightenment (x)

%package fb
Summary:    Enlightened Core X interface library (fb)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description fb
Core abstraction layer for enlightenment (fb)


%prep
%setup -q


%build
export CFLAGS+=" -fvisibility=hidden -fPIC"
export LDFLAGS+=" -fvisibility=hidden -Wl,--hash-style=both -Wl,--as-needed"

%autogen --disable-static \
	--enable-ecore-fb \
	--enable-dependency-tracking \
	--disable-ecore-directfb \
	--enable-ecore-evas-fb \
	--disable-rpath \
	--disable-openssl \
	--disable-gnutls \
	--disable-tslib \
	--enable-simple-x11 \
	--enable-ecore-evas-opengl-x11 \
	--enable-curl \
	--enable-glib-integration-always \
	--enable-ecore-x-gesture \
	--disable-xim \
	--disable-ecore-imf-xim \
	--disable-ecore-imf-scim

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-con
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-evas
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-file
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-imf
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-imf-evas
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-input
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-input-evas
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-ipc
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-x
cp %{_builddir}/%{buildsubdir}/COPYING %{buildroot}/usr/share/license/%{name}-fb

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post con -p /sbin/ldconfig

%postun con -p /sbin/ldconfig

%post evas -p /sbin/ldconfig

%postun evas -p /sbin/ldconfig


%post file -p /sbin/ldconfig

%postun file -p /sbin/ldconfig


%post imf -p /sbin/ldconfig

%postun imf -p /sbin/ldconfig


%post imf-evas -p /sbin/ldconfig

%postun imf-evas -p /sbin/ldconfig


%post input -p /sbin/ldconfig

%postun input -p /sbin/ldconfig


%post input-evas -p /sbin/ldconfig

%postun input-evas -p /sbin/ldconfig


%post ipc -p /sbin/ldconfig

%postun ipc -p /sbin/ldconfig


%post x -p /sbin/ldconfig

%postun x -p /sbin/ldconfig


%post fb -p /sbin/ldconfig

%postun fb -p /sbin/ldconfig

%files 
%defattr(-,root,root,-)
%{_libdir}/libecore.so.*
%manifest %{name}.manifest
/usr/share/locale/*
/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_includedir}/ecore-1/*.h
%{_libdir}/pkgconfig/ecore*.pc
%{_libdir}/libecore.so
%{_libdir}/libecore_con.so
%{_libdir}/libecore_evas.so
%{_libdir}/libecore_file.so
%{_libdir}/libecore_imf.so
%{_libdir}/libecore_imf_evas.so
%{_libdir}/libecore_input.so
%{_libdir}/libecore_input_evas.so
%{_libdir}/libecore_ipc.so
%{_libdir}/libecore_x.so
%{_libdir}/libecore_fb.so

%files tools
%defattr(-,root,root,-)
#/usr/bin/ecore_test
%{_bindir}/ecore_evas_convert

%files con
%defattr(-,root,root,-)
%{_libdir}/libecore_con.so.*
%manifest %{name}-con.manifest
/usr/share/license/%{name}-con

%files evas
%defattr(-,root,root,-)
%{_libdir}/libecore_evas.so.*
%manifest %{name}-evas.manifest
/usr/share/license/%{name}-evas

%files file
%defattr(-,root,root,-)
%{_libdir}/libecore_file.so.*
%manifest %{name}-file.manifest
/usr/share/license/%{name}-file

%files imf
%defattr(-,root,root,-)
%{_libdir}/libecore_imf.so.*
%manifest %{name}-imf.manifest
/usr/share/license/%{name}-imf

%files imf-evas
%defattr(-,root,root,-)
%{_libdir}/libecore_imf_evas.so.*
%manifest %{name}-imf-evas.manifest
/usr/share/license/%{name}-imf-evas

%files input
%defattr(-,root,root,-)
%{_libdir}/libecore_input.so.*
%manifest %{name}-input.manifest
/usr/share/license/%{name}-input

%files input-evas
%defattr(-,root,root,-)
%{_libdir}/libecore_input_evas.so.*
%manifest %{name}-input-evas.manifest
/usr/share/license/%{name}-input-evas

%files ipc
%defattr(-,root,root,-)
%{_libdir}/libecore_ipc.so.*
%manifest %{name}-ipc.manifest
/usr/share/license/%{name}-ipc

%files x
%defattr(-,root,root,-)
%{_libdir}/libecore_x.so.*
%manifest %{name}-x.manifest
/usr/share/license/%{name}-x

%files fb
%defattr(-,root,root,-)
%{_libdir}/libecore_fb.so.*
%manifest %{name}-fb.manifest
/usr/share/license/%{name}-fb

