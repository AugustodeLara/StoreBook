package com.example.bookStore.ui.favorites.adapter;

import android.content.Context;
import android.os.Looper;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.curlssl.R;
import com.squareup.picasso.BuildConfig;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

public class FavoritesAdapter extends RecyclerView.Adapter<FavoritesAdapter.ViewHolder> {

    private static final String TAG = "FavoritesAdapter";

    private Context context;
    private List<String> books = new ArrayList<>();

    public FavoritesAdapter(List<String> books, Context context) {
        this.books = books;
        this.context = context;
    }

    // Method to add a book to the adapter
    public void addBook(String id, String title, String description, String thumbnailUrl) {
        // Add elements to the list in the correct order
        books.add(removeDoubleAsterisks(id));
        books.add(removeDoubleAsterisks(title));
        books.add(removeDoubleAsterisks(description));
        books.add(removeDoubleAsterisks(thumbnailUrl));

        // Notify that an item has been inserted at the last position
        notifyItemInserted(books.size() / 4);
    }

    // ViewHolder class to hold the views
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView textTitle;
        public TextView textDescription;
        public ImageView imageThumbnail;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            textTitle = itemView.findViewById(R.id.textTitle);
            textDescription = itemView.findViewById(R.id.textDescription);
            imageThumbnail = itemView.findViewById(R.id.imageThumbnail);
        }
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        // Inflate the layout for the item view
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_book, parent, false);
        return new ViewHolder(view);
    }

    // Method to remove double asterisks from a string
    private String removeDoubleAsterisks(String input) {
        return input.replace("**", "");
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        // Ensure there is enough data to fill a complete set
        if (position * 4 < books.size()) {
            String title = books.get(position * 4 + 1);  // Title
            String description = books.get(position * 4 + 2);  // Description
            String thumbnailUrl = books.get(position * 4 + 3);  // Image URL

            Log.d(TAG, "XXXXXX-onBindViewHolder Title: " + title);
            Log.d(TAG, "XXXXXX-onBindViewHolder Description: " + description);
            Log.d(TAG, "XXXXXX-onBindViewHolder ThumbnailUrl: " + thumbnailUrl);

            holder.textTitle.setText(removeDoubleAsterisks(title));
            holder.textDescription.setText(removeDoubleAsterisks(description));

            // Use Picasso only if not running in a test
            if (!isRunningTest()) {
                Picasso.get().load(removeDoubleAsterisks(thumbnailUrl)).into(holder.imageThumbnail);
            }
        }
    }

    // Method to check if the code is running in a test
    private boolean isRunningTest() {
        return BuildConfig.DEBUG || Looper.myLooper() == null;
    }

    @Override
    public int getItemCount() {
        return books.size() / 4;
    }
}
