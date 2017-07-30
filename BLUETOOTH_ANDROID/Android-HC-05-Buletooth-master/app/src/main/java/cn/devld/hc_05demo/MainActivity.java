package cn.devld.hc_05demo;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.File;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final int R_DISCOVERY_DEVICE = 0xf;
    private static  final int R_SELECLT_FILE = 0x5;

    public static final UUID DEVICE_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private BluetoothDevice mDevice;
    private String mDeviceName = "未知设备";

    private BTHelper mBTHelper;

    private ScrollView mScrollView;
    private TextView tv_log;
    private EditText ev_cmd;
    private Spinner sp_br;
    private  EditText edit_file_path;
    private Button btn_send;
    private Button btn_selFile;

    public static final int WHAT_CONNECT = 0;
    public static final int WHAT_ERROR = 1;
    public static final int WHAT_RECV = 2;

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case WHAT_CONNECT:
                    boolean suc = (boolean) msg.obj;
                    if (suc) {
                        btn_send.setEnabled(true);
                        msg("连接到设备 " + mDeviceName + ".\n---------------");
                    } else {
                        msg("未连接到设备 " + mDeviceName + ".");
                    }
                    break;
                case WHAT_ERROR:
                    msg("与设备失去连接");
                    break;
                case WHAT_RECV:
                    msg((String) msg.obj);
                    break;
            }
        }
    };
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        startActivityForResult(new Intent(this, DevicesDiscoveryActivity.class), R_DISCOVERY_DEVICE);
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    private void initView() {
        edit_file_path = (EditText) findViewById(R.id.edit__select_file);
        mScrollView = (ScrollView) findViewById(R.id.main_scrollview);
        tv_log = (TextView) findViewById(R.id.main_logview);
        ev_cmd = (EditText) findViewById(R.id.main_cmdview);
        sp_br = (Spinner) findViewById(R.id.sp_br);
        btn_send = (Button) findViewById(R.id.main_send_btn);
        btn_selFile = (Button) findViewById(R.id.select_file);
        btn_selFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                try {
                    startActivityForResult(intent, R_SELECLT_FILE);
                }catch(android.content.ActivityNotFoundException e){
                    e.printStackTrace();
                }
            }
        });
        btn_send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String data = ev_cmd.getText().toString();
                ev_cmd.setText("");
                String br = "\n";
                switch (sp_br.getSelectedItemPosition()) {
                    case 0:
                        br = "\n";
                        break;
                    case 1:
                        br = "";
                        break;
                    case 2:
                        br = "\r\n";
                        break;
                }
                mBTHelper.send((data + br).getBytes(Charset.forName("GBK")));
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case R_DISCOVERY_DEVICE:
                if (resultCode == RESULT_OK) {
                    BluetoothDevice device = data.getParcelableExtra(DevicesDiscoveryActivity.EXTRA_DEVICE);
                    mDevice = device;
                    if (mDevice.getName() != null) {
                        mDeviceName = mDevice.getName();
                    }
                    msg("Device Name: " + device.getName() + " Address: " + device.getAddress());
                    mBTHelper = new BTHelper(mDevice, new BTHelper.BTListener() {
                        @Override
                        public void onConnect(boolean success) {
                            Message msg = mHandler.obtainMessage();
                            msg.what = WHAT_CONNECT;
                            msg.obj = success;
                            mHandler.sendMessage(msg);
                        }

                        @Override
                        public void onDataReceived(String data) {
                            Message msg = mHandler.obtainMessage();
                            msg.what = WHAT_RECV;
                            msg.obj = data;
                            mHandler.sendMessage(msg);
                        }

                        @Override
                        public void onError() {
                            mHandler.sendEmptyMessage(WHAT_ERROR);
                        }
                    });
                    msg("Connecting to " + mDeviceName + ".");
                    mBTHelper.connect(DEVICE_UUID);
                } else {
                    msg("No device selected.");
                }
                break;

        }
        if(resultCode == Activity.RESULT_OK){
            Uri uri = data.getData();
            String[] proj = {"MediaStore.Images.Media,DATA"};
            Cursor actualImageCursor = managedQuery(uri,proj,null,null,null);
            int index = actualImageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            actualImageCursor.moveToFirst();
            String img_path = actualImageCursor.getString(index);
            edit_file_path.setText(img_path);
            File file = new File(img_path);
            Toast.makeText(getApplicationContext(),file.toString(),Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mBTHelper != null) {
            mBTHelper.disconnect();
        }
    }

    public void msg(String msg) {
//        byte[] bt = msg.getBytes();
//        String str = new String(bt,Charset.forName("GBK"));
        tv_log.append(msg + "\n");
        mScrollView.fullScroll(ScrollView.FOCUS_DOWN);
    }

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("Main Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}