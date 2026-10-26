#!/bin/sh

# Limit nproc value to be not more than 64 to avoid build machine hangs
NP=`nproc`
NPC=$(( $NP > 64 ? 64 : $NP-1 ))

HEADERS="/lib/modules/`uname -r`/build"
ACTION="compile"

show_help() {
	echo "i915 backport infrastructure configure and compilation script"
        echo "Usage:"
        echo "  $0 [ACTION] [HEADERS_PATH]"
        echo ""
        echo "Actions:"
        echo "  configure - Run autotools and configure only (uses default headers)"
        echo "  compile   - Run autotools, configure, and compile"
        echo "  clean     - Clean build artifacts"
        echo "  help      - Show this help message"
        echo ""
        echo "Examples:"
        echo "  $0 configure /path/to/kernel/headers # configure only"
        echo "  $0 compile /path/to/kernel/headers   # Use custom headers and compile"
        echo "  $0 clean                             # Clean all build artifacts"
        echo "  $0 help                              # Show this help"
        echo ""
        echo "Default headers: /lib/modules/\$(uname -r)/build"
}

if [ ! -z "$1" ]; then
        case "$1" in
                help|-h|--help)
                        show_help
                        exit 0
                        ;;
                configure)
                        ACTION="$1"
			if [ ! -z "$2" ]; then
				HEADERS="$2"
			fi
                        ;;
                compile)
                        ACTION="$1"
                        if [ ! -z "$2" ]; then
                                HEADERS="$2"
                        fi
                        ;;
                clean)
                        ACTION="$1"
                        ;;
                *)
                        HEADERS="$1"
                        if [ ! -z "$2" ]; then
                                case "$2" in
                                        help|-h|--help)
                                                show_help
                                                exit 0
                                                ;;
                                        *)
                                                ACTION="$2"
                                                ;;
                                esac
                        fi
                        ;;
        esac
fi

echo "Using Headers from folder $HEADERS"

echo "Running Autotools"
autoreconf --install;
autoconf
autoheader
automake --add-missing
#TBD: make package creation independent from autotools
./configure --enable-linux-builtin --with-linux="$HEADERS" || exit $?

case "$ACTION" in
        compile)
                echo "Starting Compilation"
                cp src/defconfigs/i915 src/.config
                make olddefconfig
                make -j$NPC modules
                echo "Compilation Done"
                ;;
        configure)
                echo "Configuration completed (compilation skipped)"
                ;;
	#TBD : Clean leftover files after clean-up
        clean)
                echo "Cleaning build artifacts"
                make clean 2>/dev/null || true
                make mrproper 2>/dev/null || true
                make distclean 2>/dev/null || true
                echo "Clean completed"
                ;;
        *)
                echo "Invalid action: $ACTION. Use 'configure', 'compile', or 'clean'"
                exit 1
                ;;
esac

echo "Done"
