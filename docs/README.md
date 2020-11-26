# Mbed OS Sphinx Docs prototype

## Setup
* Install Doxygen and Python 3 (Mac): `brew install doxygen python3`
* Install dependencies: `pip3 install -r requirements.txt`

## Generate docs (from this directory)
* Create output directories for Doxygen: `mkdir -p dox/xml`
* Generate Doxygen docs: `pushd ..; doxygen doxyfile_options; popd`
* Generate Sphinx docs: `make html`
* Sphinx documentation is output in `_build/html`