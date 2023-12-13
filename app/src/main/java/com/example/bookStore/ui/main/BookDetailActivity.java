package com.example.bookStore.ui.main;

import android.content.Intent;
import android.content.res.ColorStateList;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.curlssl.R;
import com.squareup.picasso.Picasso;

public class BookDetailActivity extends AppCompatActivity {

    private boolean isBookFavorited = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_book_detail);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        // Extract book details from intent
        String id = getIntent().getStringExtra("id");
        String title = getIntent().getStringExtra("title");
        String description = getIntent().getStringExtra("description");
        String thumbnailUrl = getIntent().getStringExtra("thumbnailUrl");
        boolean isFavorite = isFavorite(id);
        String buyLink = getIntent().getStringExtra("buyLink");

        // Initialize UI elements
        TextView idTextView = findViewById(R.id.detailId);
        TextView titleTextView = findViewById(R.id.detailTitle);
        TextView descriptionTextView = findViewById(R.id.detailDescription);
        ImageView thumbnailImageView = findViewById(R.id.detailThumbnail);
        Button favoriteButton = findViewById(R.id.buttonFavorite);
        Button buyButton = findViewById(R.id.buttonBuy);

        idTextView.setText("ID: " + id);
        titleTextView.setText(title);
        descriptionTextView.setText(description);
        Picasso.get().load(thumbnailUrl).into(thumbnailImageView);

        // Set favorite button state and appearance
        setFavoriteButtonState(isFavorite, favoriteButton);

        favoriteButton.setOnClickListener(view -> {
            if (!isBookFavorited) {
                favoriteBook(id, title, description, thumbnailUrl);
                showToast("Livro Favoritado!");
                isBookFavorited = true;
                setFavoriteButtonState(true, favoriteButton);
            } else {
                unfollowBook(id);
                showToast("Livro Desfavoritado!");
                isBookFavorited = false;
                setFavoriteButtonState(false, favoriteButton);
            }
        });

        // Set buy button state and behavior
        setBuyButtonState(buyLink, buyButton);
    }

    private void setFavoriteButtonState(boolean isFavorite, Button favoriteButton) {
        isBookFavorited = isFavorite;
        if (isFavorite) {
            setButtonAppearance(favoriteButton, "FAVORITO", android.R.color.holo_green_light);
        } else {
            setButtonAppearance(favoriteButton, "FAVORITAR", android.R.color.holo_red_light);
        }
    }

    private void setButtonAppearance(Button button, String text, int colorResource) {
        button.setText(text);
        button.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(colorResource)));
    }

    private void showToast(String message) {
        Toast.makeText(BookDetailActivity.this, message, Toast.LENGTH_SHORT).show();
    }

    private void setBuyButtonState(String buyLink, Button buyButton) {
        if ("Indisponivel".equals(buyLink.trim())) {
            buyButton.setVisibility(View.VISIBLE);
            buyButton.setEnabled(false);
            buyButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.darker_gray)));
            buyButton.setBackgroundColor(getResources().getColor(android.R.color.darker_gray));
            buyButton.setText("Indisponível");
        } else if (buyLink != null && !buyLink.isEmpty()) {
            buyButton.setVisibility(View.VISIBLE);
            buyButton.setOnClickListener(view -> openBuyLink(buyLink));
        } else {
            buyButton.setVisibility(View.GONE);
        }
    }

    private void openBuyLink(String buyLink) {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(buyLink));
        if (browserIntent.resolveActivity(getPackageManager()) != null) {
            startActivity(browserIntent);
        } else {
            showToast("Nenhum navegador disponível");
        }
    }

    private native void favoriteBook(String id, String title, String description, String thumbnail);

    private native boolean unfollowBook(String id);

    private native boolean isFavorite(String id);

    static {
        System.loadLibrary("app");
    }
}
