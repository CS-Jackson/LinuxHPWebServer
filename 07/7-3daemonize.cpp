bool daemonize()
{
    //创建子进程，关闭父进程，从而使得程序在后台进行
    pid_t pid = fork();
    if ( pid < 0 )
    {
        return false;
    }
    else if ( pid > 0 )
    {
        exit( 0 );
    }
    //设置文件权限掩码。当进程创建文件（open），文件权限是 mode & 0777
    umask( 0 );
    //创建新会话，设置本进程为进程的组首领
    pid_t sid = setsid();
    if ( sid < 0 )
    {
        return false;
    }
    //切换工作目录
    if ( ( chdir( "/" ) ) < 0 )
    {
        /* Log the failure */
        return false;
    }
    //关闭标准输入设备、标准输出设备和标准错误输出设备
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );

    //关闭其他已经打开IDE文件描述符，如果有的话。
    //将标准输入、标准输出和标准错误输出定位到指定路径文件
    open( "/dev/null", O_RDONLY );
    open( "/dev/null", O_RDWR );
    open( "/dev/null", O_RDWR );
    return true;
}