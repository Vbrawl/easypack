# Maintainer: Vbrawl <konstantosjim@gmail.com>
pkgname=easypack-git
pkgver=0.0.1
pkgrel=1
pkgdesc=""
arch=('any')
url="https://github.com/Vbrawl/easypack"
license=('MIT')
options=()
source=('git+https://github.com/Vbrawl/easypack.git')
md5sums=('SKIP')

build() {
	cd "$srcdir/easypack"
  cmake -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
  cd build
	make -j $(nproc)
}

package() {
	cd "$srcdir/easypack/build"
	make DESTDIR="$pkgdir/" install
}
