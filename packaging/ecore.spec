Name:       ecore
Summary:    Enlightened Core X interface library
Version:    1.0.999.svn61874
Release:    1
Group:      TO_BE/FILLED_IN
License:    BSD
URL:        http://www.enlightenment.org
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(eet)
BuildRequires:  pkgconfig(evas)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(xcursor)
BuildRequires:  pkgconfig(xrender)
BuildRequires:  pkgconfig(xinerama)
BuildRequires:  pkgconfig(xrandr)
BuildRequires:  pkgconfig(xext)
BuildRequires:  pkgconfig(xi)
BuildRequires:  pkgconfig(xfixes)
BuildRequires:  pkgconfig(xcomposite)
BuildRequires:  pkgconfig(xdamage)
BuildRequires:  pkgconfig(xextproto)
BuildRequires:  pkgconfig(xtst)
BuildRequires:  pkgconfig(libcurl)
BuildRequires:  libjpeg-devel


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

%autogen
%configure --disable-static \
        --enable-glib-integration-always \
        --disable-xim

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install


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
/usr/bin/ecore_test

%files con
%defattr(-,root,root,-)
%{_libdir}/libecore_con.so.*

%files evas
%defattr(-,root,root,-)
%{_libdir}/libecore_evas.so.*

%files file
%defattr(-,root,root,-)
%{_libdir}/libecore_file.so.*

%files imf
%defattr(-,root,root,-)
%{_libdir}/libecore_imf.so.*

%files imf-evas
%defattr(-,root,root,-)
%{_libdir}/libecore_imf_evas.so.*

%files input
%defattr(-,root,root,-)
%{_libdir}/libecore_input.so.*
/usr/lib/ecore/immodules/xim.so

%files input-evas
%defattr(-,root,root,-)
%{_libdir}/libecore_input_evas.so.*

%files ipc
%defattr(-,root,root,-)
%{_libdir}/libecore_ipc.so.*

%files x
%defattr(-,root,root,-)
%{_libdir}/libecore_x.so.*

%files fb
%defattr(-,root,root,-)
%{_libdir}/libecore_fb.so.*

