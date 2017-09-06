package unstall.yyh.com.a360installtolistener;

import android.os.Build;
import android.os.FileObserver;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Html;
import android.view.View;
import android.widget.DatePicker;
import android.widget.NumberPicker;
import android.widget.TextView;
/**
 * 类功能描述：</br>
 *  NDK项目实战—高仿360手机助手之卸载监听
 * 博客地址：http://blog.csdn.net/androidstarjack
 * 公众号：终端研发部
 * @author yuyahao
 * @version 1.0 </p> 修改时间：</br> 修改备注：</br>
 */
public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        final TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(Html.fromHtml("点我试试看：\n<br><br>" +
                "        NDK项目实战—高仿360手机助手之卸载监听"));
        callUnInstallListener(Build.VERSION.SDK_INT,"data/data/unstall.yyh.com.a360installtolistener");
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv.setText(Html.fromHtml(stringFromJNI()));
                DatePicker s;
                NumberPicker ss;
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 自己写的一个监听的本地方法
     * @param versionSdk
     * @param path
     */
    private native void callUnInstallListener(int versionSdk,String path);
}
