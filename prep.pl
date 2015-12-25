#!/usr/bin/env perl

use 5.010;
use strict;
use warnings;

use Cwd qw(abs_path);
use File::Basename qw(dirname);
use File::Fetch;
use File::Spec::Functions;
use Archive::Extract;

my $script_dir = dirname(abs_path($0));
my $SDL_FNAME = 'SDL2-devel-2.0.3-VC.zip';
my $SDL_URL = "http://libsdl.org/release/$SDL_FNAME";

# Download SDL development archive
say "Downloading SDL";
my $ff = File::Fetch->new(uri => $SDL_URL);
my $where = $ff->fetch(to => $script_dir);
if ($where) {
    say "File downloaded: $where";
} else {
    die "FAILED";
}

# Unpack SDL archive
say "Unpacking";
my $sdl_file = catfile($script_dir, $SDL_FNAME);
my $ae = Archive::Extract->new(archive => $sdl_file);
$ae->extract(to => $script_dir) or die "Failed to extract archive";

# Remove archive
say "Removing archive";
unlink $sdl_file or warn "Failed to remove SDL archive";
