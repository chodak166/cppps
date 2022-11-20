#!/bin/bash

# env variables:
# OWNER - output package owner UID
# OUT_DIR - package output directory name

CPPPS_DIR=/usr/src/cppps

main()
{
  makePackage
  makeTests
  installPackages
  buildMinimalExample
  buildLoggerExample
}

makePackage()
{
  pushd $CPPPS_DIR
  mkdir -p ${OUT_DIR}/source 2>/dev/null ||:
  dpkg-buildpackage -us -uc -ui -i -I --build=source
  mv -v $CPPPS_DIR/../*.* ${OUT_DIR}/source
  dpkg-buildpackage -us -uc -ui -i -I --build=binary
  mv -v $CPPPS_DIR/../*.* ${OUT_DIR}/
  chown -v -R ${OWNER}:${OWNER} ${OUT_DIR}
  popd
}

installPackages()
{
  pushd $CPPPS_DIR/${OUT_DIR}/
  apt install -y ./libcppps*.deb
  popd
}

makeTests()
{
  pushd $CPPPS_DIR
  dir=build-$(date "+%y%m%d_%H%M%S")
  mkdir $dir && cd $dir
  cmake -DCMAKE_BUILD_TYPE=Release \
    -DCPPPS_DL_BUILD=ON \
    -DCPPPS_LOGGING_BUILD=ON \
    -DCPPPS_LOGGING_USE_ELPP=ON \
    -DCPPPS_LOGGING_BUILD_DL_PLUGIN=ON \
    -DCPPPS_DL_UNIT_TESTS_BUILD=ON \
    -DCPPPS_DL_INTEGRATION_TESTS_BUILD=ON \
    ..
  cmake --build . -- -j4
  ctest .
  popd
}

buildMinimalExample()
{
  pushd $CPPPS_DIR/examples/minimal
  dir=build-$(date "+%y%m%d_%H%M%S")
  mkdir $dir && cd $dir
  cmake -DCMAKE_BUILD_TYPE=Release ..
  cmake --build . -- -j4
  ./bin/cppps-minimal
  popd
}

buildLoggerExample()
{
  pushd $CPPPS_DIR/examples/shared_logger
  dir=build-$(date "+%y%m%d_%H%M%S")
  mkdir $dir && cd $dir
  cmake -DCMAKE_BUILD_TYPE=Release ..
  cmake --build . -- -j4
  ./bin/cppps-shared-logger
  popd
}

main $@
