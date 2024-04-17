require.config({
    baseUrl: '/crawl/static/scripts',
    shim: {
        'client': ['jquery']
    },
    paths: {
        'jquery': '/crawl/static/scripts/contrib/jquery'
    },
    waitSeconds: 0
});

require(['client']);
