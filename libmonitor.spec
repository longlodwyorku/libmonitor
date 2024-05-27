Name:           libmonitor
Version:        0
Release:        1%{?dist}
Summary:        Dynamic library for getting cpu and memory usage

License:        MIT
URL:            https://github.com/longlodwyorku/libmonitor
Source0:        %URL/archive/refs/tags/v%{version}.tar.gz

BuildRequires: g++
BuildRequires: make

%description
The %{name} package contains libraries for
applications that use %{name}.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%autosetup


%build
%configure --disable-static
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%{?ldconfig_scriptlets}


%files
%license add-license-file-here
%doc add-main-docs-here
%{_libdir}/*.so.*

%files devel
%doc add-devel-docs-here
%{_includedir}/*
%{_libdir}/*.so


%changelog
* Sun May 26 2024 longlodw@my.yorku.ca
- 
