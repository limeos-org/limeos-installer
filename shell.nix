{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gnumake
    clang # C compiler
    ncurses
    pkg-config # Helper for finding libraries
  ];
}
