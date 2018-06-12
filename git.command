cd AIProject
make distclean
cd ..
cd MyCV
make distclean
cd ..

cd SharUI
make distclean
cd ..

make distclean
cd ..

cd DFApp
git add *
git commit -m "update"
git push
git pull