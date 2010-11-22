#! /usr/bin/perl
#
# Purpose:
#
#    obj2opengl.pl converts an OBJ file to an OpenGL .H file.
#
#  Discussion:
#
#    The script reads the vertices and faces from the OBJ file,
#    and computes vertex normals averaged from the faces.
#
#    To use the resulting .h file requires a companion file
#    called graphicDefs.h.
#
#  Modified:
#
#    12 January 2003
#
#  Author:
#
#    Margaret Geroch
# 
if ( $#ARGV != 0 ) 
{
  print "usage: obj2opengl.pl <fname>.obj\n";
  exit;
}
#
#  Assign the name of the input file.
#
$inFilename = $ARGV[0];

if ( $inFilename =~ /(\w+)\.obj/) 
{
  $object = $1;
}
#
#  Open the input file.
#
open ( INFILE, "<$inFilename" ) 
  || die "Can't find file $inFilename...exiting \n";
#
#  Set the output file name.
#
$outFilename = $object.".h";
#
#  Report on the conversion to take place.
#
print "\n";
print "  Input:  the obj file: $inFilename\n";
print "  Output: the C/C++ header file $outFilename with vertex normals\n";

$numVerts = 0;
$numFaces = 0;
#
#  Read in each line.
#    Count the vertices and faces.
#    Find the sum, max and min for each component.
#
while ( $line = <INFILE> ) 
{
  chop $line;
  
  if ($line =~ /v\s+.*/)
  {
  
    $numVerts++;
    @tokens = split(' ', $line);
    
    $xsum += $tokens[1];
    $ysum += $tokens[2];
    $zsum += $tokens[3];
    
    if ( $numVerts == 1 )
    {
      $xmin = $tokens[1];
      $xmax = $tokens[1];
      $ymin = $tokens[2];
      $ymax = $tokens[2];
      $zmin = $tokens[3];
      $zmax = $tokens[3];
    }
    else
    {   
        if ($tokens[1] < $xmin)
      {
        $xmin = $tokens[1];
      }
      elsif ($tokens[1] > $xmax)
      {
        $xmax = $tokens[1];
      }
    
      if ($tokens[2] < $ymin) 
      {
        $ymin = $tokens[2];
      }
      elsif ($tokens[2] > $ymax) 
      {
        $ymax = $tokens[2];
      }
    
      if ($tokens[3] < $zmin) 
      {
        $zmin = $tokens[3];
      }
      elsif ($tokens[3] > $zmax) 
      {
        $zmax = $tokens[3];
      }
    
    }
 
  }
 
  elsif ($line =~ /f\s+.*/)
  {
    $numFaces++;
  }
}

close INFILE;
#
#  Calculate the center and the scale factor
#
$xcen = $xsum / $numVerts;
$ycen = $ysum / $numVerts;
$zcen = $zsum / $numVerts;

$xdiff = ($xmax - $xmin);
$ydiff = ($ymax - $ymin);
$zdiff = ($zmax - $zmin);

if ( ( $xdiff >= $ydiff ) && ( $xdiff >= $zdiff ) ) 
{
  $scalefac = $xdiff;
}
elsif ( ( $ydiff >= $xdiff ) && ( $ydiff >= $zdiff ) ) 
{
  $scalefac = $ydiff;
}
else 
{
  $scalefac = $zdiff;
}
$scalefac = 1.0 / $scalefac;
#
#  Create variable names for the information.
#
if ( $object =~ /(\w)(\w*)/) 
{
  $let = $1;
  $morelets = $2;
  $let = "\u$let";
}
$object = $let.$morelets;
#
#  Open the output file and put in some preliminary information.
#
open ( OUTFILE, ">$outFilename" ) 
  || die "Can't create file $outFilename ... exiting\n";
  
print OUTFILE;
$str = "#include QgraphicsDefs.hQ\n\n";
$str =~ s/Q/\"/;
$str =~ s/Q/\"/;
print OUTFILE $str;
$str = "int num".$object."Verts = ".$numVerts.";\n\n";
print OUTFILE $str;
$str = "point3 ".$object."Verts \[\] = {\n"; 
print OUTFILE $str;

#
#  Open the input file,  and get the points and move
#  them so that the object is centered at the origin, and then scale
#  them so the object fits in the unit block (-0.5...0.5)
#  before printing them to the output file
#
open ( INFILE, "<$inFilename" )
  || die "Can't find file $inFilename...exiting \n";

$j = 0;

while ( $j < $numVerts ) 
{

  $line = <INFILE>;
  chop $line;
  
  if ($line =~ /v\s+.*/)
  {
    @tokens= split(' ', $line);
    $x = ( $tokens[1] - $xcen ) * $scalefac;
    $y = ( $tokens[2] - $ycen ) * $scalefac;
    $z = ( $tokens[3] - $zcen ) * $scalefac;

    printf(OUTFILE "     {%.5f, %.5f, %.5f},\n", $x, $y, $z);
#
#  Save the vertex coordinates.
#
    $xcoords[$j] = $x;
    $ycoords[$j] = $y;
    $zcoords[$j] = $z;
#
#  Zero out the normal information.
#
    $nx[$j] = $ny[$j] = $nz[$j] = $nj[$j] = 0;
    
    $j = $j + 1;
  }
}
close INFILE;
#
#  Open the input file to get information for the normal vectors.
#
print OUTFILE " };\n\n";
print OUTFILE "int num".$object."Faces = ".$numFaces.";\n\n";
print OUTFILE "int ".$object."Faces \[\]\[3\] = {\n";

open ( INFILE, "<$inFilename" )
  || die "Can't find file $inFilename...exiting \n";
  
$j = 0;

while ( $j < $numFaces ) 
{
  $line = <INFILE>;
  chop $line;
  
  if ($line =~ /f (\d+) (\d+) (\d+)/) 
  {
    $index0 = $1-1;
    $index1 = $2-1;
    $index2 = $3-1;
    print OUTFILE "     {$index0, $index1, $index2},\n";
#
#  Calculate the normal for this face and add it
#  to the running sum of normals for each of the
#  three vertices involved
#
    $dx1 = $xcoords[$index1] - $xcoords[$index0];
    $dy1 = $ycoords[$index1] - $ycoords[$index0];
    $dz1 = $zcoords[$index1] - $zcoords[$index0];
    $dx2 = $xcoords[$index2] - $xcoords[$index0];
    $dy2 = $ycoords[$index2] - $ycoords[$index0];
    $dz2 = $zcoords[$index2] - $zcoords[$index0];
    $vx = $dy1*$dz2 - $dz1*$dy2;
    $vy = $dz1*$dx2 - $dx1*$dz2;
    $vz = $dx1*$dy2 - $dy1*$dx2;
    $d = sqrt ( $vx*$vx + $vy*$vy + $vz*$vz );

    if ( $d == 0 )
    {
      $vx = 1;
      $vy = 0;
      $vz = 0;
    }
    else
    {
      $vx /= $d;
      $vy /= $d;
      $vz /= $d;
    }
    
    $nx[$index0] += $vx;
    $ny[$index0] += $vy;
    $nz[$index0] += $vz;
    $nj[$index0] += 1;
    $nx[$index1] += $vx;
    $ny[$index1] += $vy;
    $nz[$index1] += $vz;
    $nj[$index1] += 1;
    $nx[$index2] += $vx;
    $ny[$index2] += $vy;
    $nz[$index2] += $vz;
    $nj[$index2] += 1; 
    
    $j = $j + 1;
  }
}
close INFILE;

print OUTFILE "};\n\n";
#
#  Normalize the vertex normal vectors.
#
for ( $j = 0; $j < $numVerts; ++$j) 
{
 $d = sqrt ( $nx[$j]*$nx[$j] + $ny[$j]*$ny[$j] + $nz[$j]*$nz[$j] );
  
  if ( $d == 0 )
  {
    $nx[$j] = 1;
    $ny[$j] = 0;
    $nz[$j] = 0;
  }
  else
  {
    $nx[$j] = $nx[$j] / $d;
    $ny[$j] = $ny[$j] / $d;
    $nz[$j] = $nz[$j] / $d;
  }
    
}
#
#  Write the vertex normals to the file.
#
$str = "point3 ".$object."VertNorms \[\] = {\n"; 
print OUTFILE $str;

for ( $j = 0; $j < $numVerts; ++$j) 
{
  print OUTFILE "     {", $nx[$j], ", ", $ny[$j], ", ", $nz[$j], "},\n";
}

print OUTFILE "};\n";

close OUTFILE;
