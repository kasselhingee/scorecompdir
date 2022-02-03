# Simulates then estimates for model 7 of section A.11 in the JASA paper


#choose seed
set.seed(1)

#dimension
p=3

#Dirichlet parameters (beta+1)
alpha=matrix(0.50,p,1)
alpha[2]=1.70
alpha[3]=541
alpha-1

#sample size
n=92

#simulate sample from Dirichlet distribution
dirprop=MCMCpack::rdirichlet(n, alpha)

#simulate from Dirichlet-multinomial model
ni=matrix(2000,n,1)
ni=as.vector(ni)
x=matrix(0,n,p)
for (j in 1:n)
{
  x[j,]=rmultinom(1,ni[j],prob=dirprop[j,])
}
prop=x/ni

#use estimated proportions (prop) or true proportions (dirprop) in estimation
#dirfit=prop
dirfit=dirprop

test_that("Dirichlet moment estimator runs", {
  estimate6=cdabyppi:::dirichmom(dirfit)-1
  expect_gt(mean(abs(estimate6 - alpha) < abs(alpha)) , 0.6)
})

test_that("Score2 runs", {
  #a_c for h function (any large value):
  acut=10


  #calculate scoring estimate:
  estimate3=cdabyppi:::estimator2_dir(dirfit,acut)
  expect_gt(mean(abs(estimate3 - alpha) < abs(alpha)) , 0.6)
})


test_that("Score2ac runs", {
  #a_c for h function:
  acut=0.001

  #calculate scoring estimate:
  estimate2=cdabyppi:::estimator2_dir(dirfit,acut)
  expect_gt(mean(abs(estimate2 - alpha) < abs(alpha)) , 0.6)
})

test_that("Score1 runs", {
  #a_c for h function (any large value):
  acut=10

  #calculate scoring estimate:
  estimate4=cdabyppi:::estimator1_dir(dirfit,acut)
  expect_gt(mean(abs(estimate4 - alpha) < abs(alpha)) , 0.6)
})

test_that("Score1ac runs", {
  #a_c for h function:
  acut=0.01


  #calculate scoring estimate:
  estimate1=cdabyppi:::estimator1_dir(dirfit,acut)
  expect_gt(mean(abs(estimate1 - alpha) < abs(alpha)) , 0.6)
})

