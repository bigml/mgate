mgate - fastcgi worker of moon

moon is a highy concurrency dynamic web solution write in c
(currently by bigmaliang@gmail.com)



#### The main skeleton of moon ####

![skeleton](https://raw.githubusercontent.com/bigml/mbase/master/doc/pic/skeleton.png)



#### The moon's code modules ####

![modules](https://raw.githubusercontent.com/bigml/mbase/master/doc/pic/module.png)



### mgate ###
Like [YII](	http://www.yiiframework.com/), [ThinkPHP](http://www.thinkphp.cn/) write in
PHP, mgate is another web MVC framework write in c.


#### why another web framework in c? ####
Web framework is so simple. And I'm familar with C. So, there is mgate.

Mgate is developed for high speed web application, it is hard to learn(compare with
PHP), and not very easy to agile develop. So, if you just want to copy a web site as soon
as possible(3 days for a milk farmer's site), mgate is not suitable for you.


#### directory structure ####
    mgate
    ├── walk        model. database operation
    ├── fly         view. home directory of you site. (html, js, css, image...)
    ├── run         controller. dynamic web page/interface handler. just a viki cgi.
    ├
    ├── mevent      model(for high concurrency). refer mevent/README for further info.
    ├
    ├── tpl         html templates directory. commonly used by run/ and pager/
    ├── pager       tools produce static html. like cms.
    ├── mtl         scripts, tools...
    `── pub         public c library

#### how to begin ####

1. git clone --recursive https://github.com/bigml/mgate.git mgate
2. cd mgate; sed -i 's/voov/PROJNAME/g' `grep -rl voov .`
3. find -name '\*voov\*'; rename them
4. mkdir -p /var/log/moon/mgate/; chmod 777 /var/log/moon/mgate/
5. refer doc/system-*.txt for how mgate work, and how config it
6. refer mevent/README, mevent/mbase/README to begin develop


#### develop contract ####

* reserved request key
  - /json/xxx: output json
  - /image/xxx: output image
  - /json/zero/image?_op=add for image upload

* reserved request parameter
  - _op: add, mod, del for REST request
  - _type_object: object parameter names of this request
    (tell server these paramter is json object rather than string),
    divide by ',' on multi json objects.
  - _upfile_data_type: the file type uploaded
    1. text    text file, accord readAsText(file) in js
    2. binstr  binary stream, accord readAsBinaryString(file) in js
    3. dataurl base64 coded string，makesure url secury，accord readAsDataURL(file) in js

  - _nmax: max number
  - _nmin: min number
  - _ntt: total number
  - _nst: start number
  - _npg: page number (_nst will overwrite _npg)
  - _npp: number perpage
