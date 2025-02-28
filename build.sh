#!/bin/bash

# readlink -f cannot work on mac
TOPDIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# 获取脚本所在的目录路径，并将其赋值给变量 TOPDIR
# ${BASH_SOURCE[0]} 是当前脚本的文件名
# dirname 获取文件所在的目录路径
# cd 切换到该目录，pwd 获取当前工作目录的绝对路径

BUILD_SH=$TOPDIR/build.sh
# 将build.sh的目录赋值给变量BUILD_SH
echo "THIRD_PARTY_INSTALL_PREFIX is ${THIRD_PARTY_INSTALL_PREFIX:=$TOPDIR/deps/3rd/usr/local}"
# 输出 THIRD_PARTY_INSTALL_PREFIX 的值。如果该变量未设置，则使用默认值 $TOPDIR/deps/3rd/usr/local

CMAKE_COMMAND="cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --log-level=STATUS"
# 定义一个 CMake 命令的基础字符串，包含 -DCMAKE_EXPORT_COMPILE_COMMANDS=1（生成编译命令数据库）和 --log-level=STATUS（设置日志级别为 STATUS）。
CMAKE_COMMAND_THIRD_PARTY="$CMAKE_COMMAND -DCMAKE_INSTALL_PREFIX=$THIRD_PARTY_INSTALL_PREFIX"
# 在基础 CMake 命令上添加 -DCMAKE_INSTALL_PREFIX 参数，指定第三方库的安装路径
CMAKE_COMMAND_MINIOB="$CMAKE_COMMAND"
# 将基础 CMake 命令赋值给 CMAKE_COMMAND_MINIOB，用于构建 MiniOB 项目。

ALL_ARGS=("$@")
# 将所有传递给脚本的参数存储在数组ALL_ARGS中
BUILD_ARGS=()
# 初始化一个空数组 BUILD_ARGS，用于存储构建相关的参数。
MAKE_ARGS=()
# 初始化一个空数组 MAKE_ARGS，用于存储 make 命令的参数。
MAKE=make
# 将 make 命令赋值给变量 MAKE

echo "$0 ${ALL_ARGS[@]}"
# 输出脚本的名称 ($0) 和所有传递给脚本的参数 (${ALL_ARGS[@]})。

# 打印脚本的使用说明
function usage
{
  echo "Usage:"
  echo "./build.sh -h"
  echo "./build.sh init # install dependence"
  echo "./build.sh clean"
  echo "./build.sh [BuildType] [--make [MakeOptions]]"
  echo ""
  echo "OPTIONS:"
  echo "BuildType => debug(default), release"
  echo "MakeOptions => Options to make command, default: -j N"

  echo ""
  echo "Examples:"
  echo "# Init."
  echo "./build.sh init"
  echo ""
  echo "# Build by debug mode and make with -j24."
  echo "./build.sh debug --make -j24"
}

# 解析传递给脚本的参数
function parse_args
{
  make_start=false
  for arg in "${ALL_ARGS[@]}"; do
    if [[ "$arg" == "--make" ]]
    then
      make_start=true
    elif [[ $make_start == false ]]
    then
      BUILD_ARGS+=("$arg")
    else
      MAKE_ARGS+=("$arg")
    fi

  done
}

# try call command make, if use give --make in command line.
function try_make
{
  if [[ $MAKE != false ]]
  then
    # use single thread `make` if concurrent building failed
    $MAKE "${MAKE_ARGS[@]}" || $MAKE
  fi
}

# create build directory and cd it.
function prepare_build_dir
{
  TYPE=$1
  mkdir -p $TOPDIR/build_$TYPE && cd $TOPDIR/build_$TYPE
}

function do_init
{
  git submodule update --init || return
  current_dir=$PWD

  MAKE_COMMAND="make --silent"

  # build libevent
  cd ${TOPDIR}/deps/3rd/libevent && \
    mkdir -p build && \
    cd build && \
    ${CMAKE_COMMAND_THIRD_PARTY} .. -DEVENT__DISABLE_OPENSSL=ON -DEVENT__LIBRARY_TYPE=BOTH && \
    ${MAKE_COMMAND} -j4 && \
    make install

  # build googletest
  cd ${TOPDIR}/deps/3rd/googletest && \
    mkdir -p build && \
    cd build && \
    ${CMAKE_COMMAND_THIRD_PARTY} .. && \
    ${MAKE_COMMAND} -j4 && \
    ${MAKE_COMMAND} install

  # build google benchmark
  cd ${TOPDIR}/deps/3rd/benchmark && \
    mkdir -p build && \
    cd build && \
    ${CMAKE_COMMAND_THIRD_PARTY} .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBENCHMARK_ENABLE_TESTING=OFF  -DBENCHMARK_INSTALL_DOCS=OFF -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -DBENCHMARK_USE_BUNDLED_GTEST=OFF -DBENCHMARK_ENABLE_ASSEMBLY_TESTS=OFF && \
    ${MAKE_COMMAND} -j4 && \
    ${MAKE_COMMAND} install

  # build jsoncpp
  cd ${TOPDIR}/deps/3rd/jsoncpp && \
    mkdir -p build && \
    cd build && \
    ${CMAKE_COMMAND_THIRD_PARTY} -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF .. && \
    ${MAKE_COMMAND} && \
    ${MAKE_COMMAND} install

  cd $current_dir
}

function do_musl_init
{
  git clone https://github.com/ronchaine/libexecinfo deps/3rd/libexecinfo || return
  current_dir=$PWD

  MAKE_COMMAND="make --silent"
  cd ${TOPDIR}/deps/3rd/libexecinfo && \
    ${MAKE_COMMAND} install && \
    ${MAKE_COMMAND} clean && rm ${TOPDIR}/deps/3rd/libexecinfo/libexecinfo.so.* && \
    cd ${current_dir}
}

function prepare_build_dir
{
  TYPE=$1
  mkdir -p ${TOPDIR}/build_${TYPE}
  rm -f build
  echo "create soft link for build_${TYPE}, linked by directory named build"
  ln -s build_${TYPE} build
  cd ${TOPDIR}/build_${TYPE}
}

function do_build
{
  TYPE=$1; shift
  prepare_build_dir $TYPE || return
  echo "${CMAKE_COMMAND_MINIOB} ${TOPDIR} $@"
  ${CMAKE_COMMAND_MINIOB} -S ${TOPDIR} $@
}

function do_clean
{
  echo "clean build_* dirs"
  find . -maxdepth 1 -type d -name 'build_*' | xargs rm -rf
}

function build {
  # 默认参数是 debug
  if [ -z "${BUILD_ARGS[0]}" ]; then
    set -- "debug"  # 如果没有参数，则设置默认值
  else
    set -- "${BUILD_ARGS[@]}"  # 否则使用 BUILD_ARGS 的第一个参数
  fi
  local build_type_lower=$(echo "$1" | tr '[:upper:]' '[:lower:]')  # 转换为小写
  echo "Build type: $build_type_lower"  # 输出构建类型

  do_build $@ -DCMAKE_BUILD_TYPE="$build_type_lower" # 调用 do_build
}


function main
{
  case "$1" in
    -h)
      usage
      ;;
    init)
      do_init
      ;;
    musl)
      do_musl_init
      ;;
    clean)
      do_clean
      ;;
    *)
      parse_args
      build
      try_make
      ;;
  esac
}

main "$@"
