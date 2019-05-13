const koaRouter = require('koa-router');
const ObjectID = require('mongodb').ObjectID;
const mgo = require('./mgo');

const password = 'xxxxxx';  // 设置命令、清除命令需要的密码，主要是用的get请求太简单，防止被爬虫或其它人误操作

const router = new koaRouter({ prefix: '/esp8266' });


router.get('/', async function (ctx, next) {
    const db = mgo.getEsp8266();
    const limit = ctx.query.limit ? parseInt(ctx.query.limit, 10) : 10;
    const r = await db.find().sort({ _id: -1 }).limit(limit).toArray();
    ctx.body = r;
});

router.get('/getCmd', async function (ctx, next) {
    const db = mgo.getEsp8266();
    const r = await db.find({ isAvailable: true }).sort({ _id: 1 }).limit(1).toArray();
    ctx.body = r;
});

router.get('/setCmd', async function (ctx, next) {
    if (ctx.query.password !== password) {
        ctx.status = 500;
        ctx.body = 'password err';
        return;
    }
    const cmd = ctx.query.cmd;
    const db = mgo.getEsp8266();
    const r = await db.insertOne({
        isAvailable: true,
        cmd: cmd,
        addTime: new Date(),
    })
    ctx.body = r;
});

router.get('/clearCmd', async function (ctx, next) {
    if (ctx.query.password !== password) {
        ctx.status = 500;
        ctx.body = 'password err';
        return;
    }
    const id = ctx.query.id;
    const db = mgo.getEsp8266();
    const r = await db.updateOne({ _id: ObjectID.createFromHexString(id) }, {
        $set: {
            isAvailable: false,
            updTime: new Date(),
        }
    });
    ctx.body = r;
});

module.exports = router;
// mgo.init()
// const koa = require('koa')
// const app = new koa()
// app.use(router.routes())
// app.listen(3000)