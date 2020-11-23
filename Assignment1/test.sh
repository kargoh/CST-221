echo "Create testdirectory"
mkdir testDirectory
echo "Directory created"
echo "Copy sample file into testdirectory"
cp hello.c testDirectory
echo "File copied"
echo "Delete testDirectory"
mkdir deletedDirectory
rmdir deletedDirectory
echo "testDirectory Deleted"
echo "Go into testDirectory"
cd testDirectory
echo "In testDirectory"
echo "Displaying content of sample"
cat hello.c
echo "Script Completed"
